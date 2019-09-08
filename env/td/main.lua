setLogFilter("X")

loadScriptFile("tilemap.lua")
loadScriptFile("gamemap.lua")

game = {
    rootIndex = 0,
    tilemap = nil,
    gamemap = nil,
    enemies = nil,
    turrets = nil,
}

game.start = function()
    game.time = 0
    game.score = 20

    game.queue = List:new()
    game.queue:pushright({ time = 0, x = 1, y = 2, tx = 10, ty = 9 })
    game.queue:pushright({ time = 1, x = 1, y = 2, tx = 10, ty = 9 })

    game.started = true
end

game.enemyFinished = function()
    game.score = game.score - 1
    print("score = " .. game.score)
    if (game.score == 0) then
        game.dead = true
    end
end

game.addEnemy = function(x, y, tx, ty)
    local enemy = newEntityParent(game.rootIndex)
    createScript(enemy, "enemy.lua")
    entities[enemy].spawn = {
        x = x,
        y = y,
        targetX = tx,
        targetY = ty,
        type = { name = "enemy_1", speed = 1 }
    }

    print("spawn " .. x .. "," .. y .. " index:" .. enemy)

    game.enemies[enemy] = true
end

game.removeEnemy = function(enemy)
    disposeEnemy(enemy)
    game.enemies[enemy] = nil
end

game.checkTileBuild = function(x, y)
    for enemy, valid in pairs(game.enemies) do
        if (valid) then
            local t = entities[enemy].transform
            local tx = math.floor(t.x + 0.5)
            local ty = math.floor(t.y + 0.5)
            if (x == tx and y == ty) then
                return false
            end
        end
    end
    return true
end

game.addTurret = function(x, y)
    local tile = game.gamemap.tiledata[x][y]
    if (tile.turret == nil and tile.build and game.checkTileBuild(x, y)) then
        local turret = newEntityParent(game.rootIndex)
        createScript(turret, "turret.lua")
        entities[turret].spawn = {
            x = x,
            y = y,
            baseType = "base_1",
            turretType = "turret_1",
            range = 3
        }
        game.turrets[turret] = true

        tile.turret = turret
        game.gamemap:refreshPath()
        return true
    end
    return false
end

game.removeTurret = function(x, y)
    local tile = game.gamemap.tiledata[x][y]
    if (tile.turret ~= nil) then
        game.tilemap:setTile(x, y, "terrain_2")
        disposeTurret(tile.turret)
        game.turrets[tile.turret] = nil
        tile.turret = nil
        game.gamemap:refreshPath()
        return true
    end
    return false
end

function load()
    local aspect = screenWidth / screenHeight
    local transform = createTransform(entity.index)
    local tileWidth = 10 / 2
    local tileHeight = 10 / 2
    transform.x = -(1 + 0.5 / tileWidth) / aspect
    transform.y = 1 + 0.5 / tileHeight
    transform.z = 0
    transform.sx = 1 / aspect / tileWidth
    transform.sy = -1 / tileHeight
    transform.sz = 0.1

    game.rootIndex = entity.index

    game.tilemap = TileMap:new("map.txt", entity.index)
    local tileNameMapping = {}
    tileNameMapping["1"] = "terrain_1"
    tileNameMapping["2"] = "terrain_2"
    tileNameMapping["3"] = "terrain_3"
    tileNameMapping["4"] = "terrain_4"
    game.tilemap:makeTiles(tileNameMapping)

    local roadTiles = {}
    roadTiles["2"] = true
    local buildTiles = {}
    buildTiles["2"] = true
    game.gamemap = GameMap:new(game.tilemap, roadTiles, buildTiles, { { x = 10, y = 9, dir = GameMap.PathDirections.right } })
    game.gamemap:calcPath()

    game.enemies = {}
    game.turrets = {}

    game.start()
end

function update()
    if (game.started and game.finished) then
        return
    end

    game.time = game.time + dt

    --input
    local m1 = mouse1down()
    local m2 = mouse2down()
    local m3 = mouse3down()
    do
        local t = entity.transform
        local mx = inputStateCurr.mouseX / screenWidth * 2 - 1
        local my = 1 - inputStateCurr.mouseY / screenHeight * 2
        local x = (mx - t.x) / t.sx
        local y = (my - t.y) / t.sy
        local tileX = math.floor(x + 0.5)
        local tileY = math.floor(y + 0.5)

        game.mouse = {
            x = x,
            y = y,
            tileX = tileX,
            tileY = tileY
        }
    end

    --check enemy finish
    for enemy, valid in pairs(game.enemies) do
        if (valid) then
            if (entities[enemy].finished) then
                game.enemyFinished()
                entities[enemy].dead = true
            end
        end
    end

    --check enemy dead
    local enemyLeft = not game.queue:isEmpty()
    if (not enemyLeft) then
        for enemy, valid in pairs(game.enemies) do
            if (valid) then
                if (entities[enemy].dead) then
                    game.removeEnemy(enemy)
                else
                    enemyLeft = true
                end
            end
        end
    end

    --check level finish
    if (game.dead) then
        game.finished = true
        print("dead")
    end
    if (not game.dead and not enemyLeft) then
        game.finished = true
        print("victory")
    end

    --spawn enemy
    if (not game.queue:isEmpty()) then
        local next = game.queue:getleft()
        if (game.time >= next.time) then
            game.queue:popleft()
            game.addEnemy(next.x, next.y, next.tx, next.ty)
        end
    end

    --left click to add turret, right to remove turret
    if (m1 or m3) then
        local tileX = game.mouse.tileX
        local tileY = game.mouse.tileY
        if (tileX > 0 and tileX <= game.gamemap.width and tileY > 0 and tileY <= game.gamemap.height) then
            if (m1) then
                game.addTurret(tileX, tileY)
            end
            if (m3) then
                game.removeTurret(tileX, tileY)
            end
        end
    end

    if (m2) then
        local tileX = game.mouse.tileX
        local tileY = game.mouse.tileY
        game.addEnemy(tileX, tileY, 10, 9)
    end
end
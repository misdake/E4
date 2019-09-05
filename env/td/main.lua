setLogFilter("file")

loadScriptFile("tilemap.lua")
loadScriptFile("gamemap.lua")

game = {}

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

    local enemy = newEntityParent(game.rootIndex)
    createScript(enemy, "enemy.lua")
    entities[enemy].spawn = {
        x = 1,
        y = 2,
        targetX = 10,
        targetY = 9,
        type = { name = "enemy_1", speed = 1 }
    }

    game.enemies = {}
    table.insert(game.enemies, enemy)

    --local turret = newEntityParent(game.rootIndex)
    --createScript(turret, "turret.lua")
    --entities[turret].spawn = {
    --    x = 5,
    --    y = 5,
    --    baseType = "base_1",
    --    turretType = "turret_1",
    --    range = 2
    --}
    game.turrets = {}
    --table.insert(game.turrets, turret)
end

function update()
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


    local m1 = mouse1down()
    local m2 = mouse2down()
    local m3 = mouse3down()
    if (m1 or m3) then
        local tileX = game.mouse.tileX
        local tileY = game.mouse.tileY
        if (tileX > 0 and tileX <= game.gamemap.width and tileY > 0 and tileY <= game.gamemap.height) then
            if (m1) then
                game.gamemap:addTurret(tileX, tileY)
            end
            if (m3) then
                game.gamemap:removeTurret(tileX, tileY)
            end
        end
    end

    if (m2) then
        entities[game.enemies[1]].transform.x = 1
        entities[game.enemies[1]].transform.y = 2
        entities[game.enemies[1]].move = nil
    end
end
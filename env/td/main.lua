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
    game.gamemap = GameMap:new(game.tilemap, roadTiles, {}, { { x = 10, y = 9, dir = GameMap.PathDirections.right } })
    game.gamemap:calcPath()

    local enemy = newEntityParent(entity.index)
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

    local turret = newEntityParent(entity.index)
    createScript(turret, "turret.lua")
    entities[turret].spawn = {
        x = 5,
        y = 5,
        baseType = "base_1",
        turretType = "turret_1"
    }
    game.turrets = {}
    table.insert(game.turrets, turret)
end

function update()
    if (mouse1down()) then
        local t = entity.transform
        local mx = inputStateCurr.mouseX / screenWidth * 2 - 1
        local my = 1 - inputStateCurr.mouseY / screenHeight * 2
        local x = (mx - t.x) / t.sx
        local y = (my - t.y) / t.sy
        local tileX = math.floor(x + 0.5)
        local tileY = math.floor(y + 0.5)
        print("mx:" .. mx .. " my:" .. my)
        print("x:" .. x .. " y:" .. y)
        print("tx:" .. tileX .. " ty:" .. tileY)

        game.gamemap:setTile(tileX, tileY)
    end
end
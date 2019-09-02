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
    game.gamemap = GameMap:new(game.tilemap, roadTiles, {})
    game.gamemap:calcPath({ { x = 10, y = 9, dir = GameMap.PathDirections.right } })

    local enemy = newEntityParent(entity.index)
    createScript(enemy, "enemy.lua")
    entities[enemy].spawn = {
        x = 1,
        y = 2,
        targetX = 10,
        targetY = 9,
        type = { name = "enemy_1", speed = 5 }
    }

end

function update()

end
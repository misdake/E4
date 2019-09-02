setLogFilter("file")

loadScriptFile("tilemap.lua")
loadScriptFile("gamemap.lua")

function load()
    local aspect = screenWidth / screenHeight
    local transform = createTransform(entity.index)
    local tileWidth = 10 / 2
    local tileHeight = 10 / 2
    transform.x = -1 / aspect
    transform.y = 1
    transform.z = 0
    transform.sx = 1 / aspect / tileWidth
    transform.sy = -1 / tileHeight
    transform.sz = 0.1

    entity.tilemap = TileMap.newFromFile("map.txt", entity.index)
    local tileNameMapping = {}
    tileNameMapping["1"] = "terrain_1"
    tileNameMapping["2"] = "terrain_2"
    tileNameMapping["3"] = "terrain_3"
    tileNameMapping["4"] = "terrain_4"
    TileMap.makeTiles(entity.tilemap, tileNameMapping)

    local roadTiles = {}
    roadTiles["2"] = true
    entity.gamemap = GameMap.init(entity.tilemap, roadTiles, {})
    GameMap.calcPath(entity.gamemap, { { x = 10, y = 9, dir = GameMap.PathDirections.right } })
end

function update()

end
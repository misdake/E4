Map = {}

function split(s, delimiter)
    result = {};
    for str in string.gmatch(s, "([^" .. delimiter .. "]+)") do
        table.insert(result, str)
    end
    return result;
end
function Map.newFromFile(filename)
    --load file
    local content = readFile(filename)
    local array = split(content, "%s")
    local width = tonumber(array[1])
    local height = tonumber(array[2])

    --read map from content
    local map = {}
    for x = 1, width do
        map[x] = {}
    end
    local index = 3
    for y = 1, height do
        for x = 1, width do
            map[x][y] = array[index]
            index = index + 1
        end
    end

    return {
        width = width,
        height = height,
        matrix = map
    }
end

TileMap = {}

function TileMap.newFromFile(filename, parent)
    --load map from file
    local map = Map.newFromFile(filename)

    --create root entity
    local root = newEntityNameParent("tilemap_root", parent)
    local transform = createTransform(root)
    local drawable = createDrawable(root)
    drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture("sprites.txt")
    drawable.visible = false

    --construct tilemap object
    local r = {}
    r.rootEntity = root
    r.map = map
    r.res = {
        mesh = drawable.mesh,
        material = drawable.material
    }
    return r
end

function TileMap.makeTiles(tileMap, tileNameMapping)
    tileMap.mapEntities = {}
    for x = 1, tileMap.map.width do
        tileMap.mapEntities[x] = {}
        for y = 1, tileMap.map.height do

            local e = newEntityParent(tileMap.rootEntity)
            local transform = createTransform(e)
            transform.x = x - 0.5
            transform.y = y - 0.5
            local drawable = createDrawable(e)
            drawable.mesh = tileMap.res.mesh
            drawable.material = newMaterialTexture("sprites.txt")
            setMaterialTextureTile(drawable.material, tileNameMapping[tileMap.map.matrix[x][y]])

            tileMap.mapEntities[x][y] = e
        end
    end
end

function TileMap.unload(tileMap)
    if (tileMap.mapEntities) then
        for y = 1, tileMap.map.height do
            for x = 1, tileMap.map.width do
                deleteEntity(tileMap.mapEntities[x][y])
            end
        end
    end

    deleteEntity(tileMap.rootEntity)
end
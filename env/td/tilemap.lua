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

function TileMap:new(filename, parent)
    local r = {}
    setmetatable(r, self)
    self.__index = self

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
    r.rootEntity = root
    r.map = map
    r.res = {
        mesh = drawable.mesh,
        material = drawable.material
    }
    return r
end

function TileMap:makeTiles(tileNameMapping)
    self.mapEntities = {}
    for x = 1, self.map.width do
        self.mapEntities[x] = {}
        for y = 1, self.map.height do

            local e = newEntityParent(self.rootEntity)
            local transform = createTransform(e)
            transform.x = x - 0.5
            transform.y = y - 0.5
            local drawable = createDrawable(e)
            drawable.mesh = self.res.mesh
            drawable.material = newMaterialTexture("sprites.txt")
            setMaterialTextureTile(drawable.material, tileNameMapping[self.map.matrix[x][y]])

            self.mapEntities[x][y] = e
        end
    end
end

function TileMap:unload()
    if (self.mapEntities) then
        for y = 1, self.map.height do
            for x = 1, self.map.width do
                deleteEntity(self.mapEntities[x][y])
            end
        end
    end

    deleteEntity(self.rootEntity)
end
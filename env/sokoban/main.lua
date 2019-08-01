--entity.mapwidth
--entity.mapheight
--entity.wall[][]
--entity.crate[][]
--entity.target[].xy
--entity.player.xy

setWindowTitle("Sokoban")

wallMask = 1
crateMask = 2
targetMask = 4
playerMask = 8

function split(s, delimiter)
    result = {};
    for str in string.gmatch(s, "([^" .. delimiter .. "]+)") do
        table.insert(result, str)
    end
    return result;
end

function loadMapFile(name)
    local content = readFile(name)
    local array = split(content, "%s")
    local width = tonumber(array[1])
    local height = tonumber(array[2])

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

    loadMap(width, height, map)
end

function loadMap(width, height, array)
    entity.mapwidth = width
    entity.mapheight = height
    entity.wall = {}
    entity.crate = {}
    entity.target = {}
    entity.player = {}
    for x = 1, width do
        entity.wall[x] = {}
        entity.crate[x] = {}
    end
    for y = 1, height do
        for x = 1, width do
            local v = array[x][y]
            entity.wall[x][y] = v & wallMask > 0
            entity.crate[x][y] = v & crateMask > 0
            if (v & targetMask > 0) then
                table.insert(entity.target, { x = x, y = y })
            end
            if (v & playerMask > 0) then
                entity.player.x = x
                entity.player.y = y
            end
        end
    end
end

function printState()
    local w = entity.mapwidth
    local h = entity.mapheight
    print("width = " .. w .. " height = " .. h)
    print("player = (" .. entity.player.x .. ", " .. entity.player.y .. ")")
    for key, value in pairs(entity.target) do
        print("target " .. key .. " = (" .. value.x .. ", " .. value.y .. ")")
    end
end

--///////////////////////////////////////////////////////////////////////////////////
--///////////////////////////////////////////////////////////////////////////////////
--///////////////////////////////////////////////////////////////////////////////////

--entity.tileEntities
--entity.targetEntities
--entity.meshPlane
--entity.matWall
--entity.matGround
--entity.matCrate
--entity.matCratePoint
--entity.matPoint
--entity.matPlayer

function setTransform(transform, x, y, sx, sy)
    local w = entity.mapwidth
    local h = entity.mapheight
    local m = math.max(w, h)
    local s = 2 / m

    transform.x = (x - 0.5) / m * 2 - 1 + (m - w) / m;
    transform.y = 1 - (y - 0.5) / m * 2 - (m - h) / m;
    transform.z = 0;
    transform.sx = s * sx;
    transform.sy = s * sy;
    transform.parent = entity.index;
end

function loadMesh()
    local e = newEntity()
    local drawable = createDrawable(e)
    drawable.mesh = newMesh("builtin:plane")
    return drawable.mesh
end
function loadMaterial(textureName)
    local e = newEntity()
    local drawable = createDrawable(e)
    --drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture(textureName)
    return drawable.material
end

function loadResources()
    entity.meshPlane = loadMesh()
    entity.matWall = loadMaterial("wall.png")
    entity.matGround = loadMaterial("ground.png")
    entity.matCrate = loadMaterial("crate.png")
    entity.matCratePoint = loadMaterial("crate_point.png")
    entity.matPoint = loadMaterial("point.png")
    entity.matPlayer = loadMaterial("player.png")
end

function loadTile(x, y)
    local e = newEntity()
    local transform = createTransform(e)
    setTransform(transform, x, y, 1, 1)
    local drawable = createDrawable(e)
    drawable.mesh = entity.meshPlane
    --drawable.material = newMaterialTexture(textureName) -- set in update
    return e
end
function loadTiles()
    entity.tileEntities = {}
    for x = 1, entity.mapwidth do
        entity.tileEntities[x] = {}
        for y = 1, entity.mapheight do
            entity.tileEntities[x][y] = loadTile(x, y)
        end
    end
end

function loadTargets()
    entity.targetEntities = {}

    for _, value in pairs(entity.target) do
        local e = newEntity()
        local transform = createTransform(e)
        setTransform(transform, value.x, value.y, 0.5, 0.5)
        transform.z = -0.2;
        local drawable = createDrawable(e)
        drawable.mesh = entity.meshPlane
        drawable.material = entity.matPoint
        table.insert(entity.targetEntities, e)
    end
end

function loadPlayer()
    local e = newEntity()
    createTransform(e)
    local drawable = createDrawable(e)
    drawable.mesh = entity.meshPlane
    drawable.material = entity.matPlayer

    entity.playerEntity = e
end

function unloadTiles()
    for x = 1, entity.mapwidth do
        for y = 1, entity.mapheight do
            deleteEntity(entity.tileEntities[x][y])
        end
    end
    entity.tileEntities = {}
end
function unloadPlayer()
    deleteEntity(entity.playerEntity)
end
function unloadTargets()
    for _, value in pairs(entity.targetEntities) do
        deleteEntity(value)
    end
end

function startMap(mapindex)
    --print("try load map " .. mapindex)
    if entity.maps[mapindex] ~= nil then
        if (entity.map ~= nil) then
            unloadTiles()
            unloadPlayer()
            unloadTargets()
        end
        entity.map = mapindex
        print("load map " .. entity.maps[mapindex])
        setWindowTitle("Sokoban " .. entity.maps[mapindex])

        loadMapFile(entity.maps[mapindex])
        printState()
        loadTiles()
        loadPlayer()
        loadTargets()
    end
end

function loadMapList()
    local content = readFile("maplist.txt")
    local names = split(content, "%s")
    entity.maps = names
end

function load()
    loadMapList()
    loadResources()
    entity.transform.sx = screenHeight / screenWidth
    startMap(1)
end


--///////////////////////////////////////////////////////////////////////////////////
--///////////////////////////////////////////////////////////////////////////////////
--///////////////////////////////////////////////////////////////////////////////////

function updateTiles()
    for x = 1, entity.mapwidth do
        for y = 1, entity.mapheight do
            local e = entities[entity.tileEntities[x][y]]
            local drawable = e.drawable
            if entity.wall[x][y] then
                drawable.material = entity.matWall
            else
                drawable.material = entity.matGround
            end
            if entity.crate[x][y] then
                drawable.material = entity.matCrate
            end
        end
    end

    for index, value in pairs(entity.target) do
        local tile = entities[entity.tileEntities[value.x][value.y]]
        local target = entities[entity.targetEntities[index]]
        if entity.crate[value.x][value.y] then
            target.drawable.visible = false
            tile.drawable.material = entity.matCratePoint
        else
            target.drawable.visible = true
        end
    end
end

function updatePlayer()
    local transform = entities[entity.playerEntity].transform
    setTransform(transform, entity.player.x, entity.player.y, 37 / 64, 59 / 64)
    transform.z = -0.1;
end

function inBound(x, y)
    if (x < 1 or x > entity.mapwidth) then
        return false
    end
    if (y < 1 or y > entity.mapheight) then
        return false
    end
    return true
end
function isEmpty(x, y)
    return inBound(x, y) and (not entity.wall[x][y]) and (not entity.crate[x][y])
end
function isCrate(x, y)
    return inBound(x, y) and entity.crate[x][y]
end

keys = { 7, 26, 4, 22 } -- D W A S
directions = { { x = 1, y = 0 }, { x = 0, y = -1 }, { x = -1, y = 0 }, { x = 0, y = 1 } } -- right up left down

function move()
    for index, value in pairs(keys) do
        if not (inputStatePrev.keys[value + 1] > 0) and (inputStateCurr.keys[value + 1] > 0) then
            local direction = directions[index]
            local nx = entity.player.x + direction.x
            local ny = entity.player.y + direction.y
            if (isCrate(nx, ny)) then
                local nnx = nx + direction.x
                local nny = ny + direction.y
                if (isEmpty(nnx, nny)) then
                    entity.player.x = nx
                    entity.player.y = ny
                    entity.crate[nx][ny] = false
                    entity.crate[nnx][nny] = true
                end
            elseif (isEmpty(nx, ny)) then
                entity.player.x = nx
                entity.player.y = ny
            end
        end
    end
end

function checkWin()
    local allset = true
    for _, value in pairs(entity.target) do
        if not entity.crate[value.x][value.y] then
            allset = false
            break
        end
    end
    return allset
end

function update()

    if not (inputStatePrev.keys[21 + 1] > 0) and (inputStateCurr.keys[21 + 1] > 0) then
        startMap(entity.map)
    end

    --printState()
    if (checkWin()) then
        startMap(entity.map + 1)
    end
    move()

    updateTiles()
    updatePlayer()
end
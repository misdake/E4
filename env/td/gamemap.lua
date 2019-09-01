loadScriptFile("list.lua")

GameMap = {}

GameMap.PathDirections = {
    unknown = { char = "X", x = 0, y = 0 },
    up = { char = "↑", x = 0, y = 1 },
    down = { char = "↓", x = 0, y = -1 },
    left = { char = "←", x = -1, y = 0 },
    right = { char = "→", x = 1, y = 0 }
}

function GameMap._createMatrixValue(width, height, value)
    local map = {}
    for x = 1, width do
        map[x] = {}
    end
    for y = 1, height do
        for x = 1, width do
            map[x][y] = value
        end
    end
    return map;
end
function GameMap._createMatrixFunc(width, height, func)
    local map = {}
    for x = 1, width do
        map[x] = {}
    end
    for y = 1, height do
        for x = 1, width do
            map[x][y] = func(x, y)
        end
    end
    return map;
end
function GameMap._matrixFillValue(map, width, height, value)
    for y = 1, height do
        for x = 1, width do
            map[x][y] = value
        end
    end
end
function GameMap._matrixFillFunc(map, width, height, func)
    for y = 1, height do
        for x = 1, width do
            map[x][y] = func(x, y)
        end
    end
end

function GameMap.init(tilemap, roadtiles, buildtiles)
    --TODO add config file as parameter

    local width = tilemap.map.width
    local height = tilemap.map.height

    local tiledata = GameMap._createMatrixFunc(width, height, function(x, y)
        local tile = tilemap.map.matrix[x][y]
        local t = { road = false, build = false, turret = nil }
        if (roadtiles[tile]) then
            t.road = true
        end
        if (buildtiles[tile]) then
            t.build = true
        end
        return t
    end)

    local directiondata = GameMap._createMatrixValue(width, height, GameMap.PathDirections.unknown)

    return {
        width = width,
        height = height,
        tiledata = tiledata,
        directiondata = directiondata
    }
end

function GameMap.pathOnTile(gamemap, x, y)
    local t = gamemap.tiledata[x][y]
    local r = t.road and (not t.turret) and true
    return r
end

function GameMap.calcPath(gamemap, dest)
    local width = gamemap.width
    local height = gamemap.height

    local MAX_PATH = 9999
    local MAX_NONPATH = 99999
    local distance = GameMap._createMatrixFunc(width, height, function(x, y)
        return GameMap.pathOnTile(gamemap, x, y) and MAX_PATH or MAX_NONPATH
    end)
    local direction = gamemap.directiondata

    local queue = List.new()

    --initialize destinations
    for _, value in pairs(dest) do
        List.pushright(queue, value)
        distance[value.x][value.y] = 0
        direction[value.x][value.y] = GameMap.PathDirections.unknown
    end

    function go(x, y, value, dir)
        if (distance[x][y] == MAX_PATH) then
            distance[x][y] = value
            List.pushright(queue, { x = x, y = y })
            direction[x][y] = dir
        end
    end

    local x_left = width - 1;
    local y_up = height - 1;

    --flood fill with queue
    while (not List.isEmpty(queue)) do
        local curr = List.popleft(queue)
        local x = curr.x
        local y = curr.y
        local value = distance[x][y] + 1
        --print("curr:" .. x .. "," .. y .. " next=" .. value)
        if (2 <= x) then
            go(x - 1, y, value, GameMap.PathDirections.right)
        end
        if (x <= x_left) then
            go(x + 1, y, value, GameMap.PathDirections.left)
        end
        if (2 <= y) then
            go(x, y - 1, value, GameMap.PathDirections.down)
        end
        if (y <= y_up) then
            go(x, y + 1, value, GameMap.PathDirections.up)
        end
    end

    local out = ""
    GameMap._matrixFillFunc(direction, width, height, function(x, y)
        out = out .. direction[x][y].char .. " "
        if (x == width) then
            print(out)
            out = ""
        end
        return direction[x][y]
    end)
end
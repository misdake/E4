function enemyWillDie(enemy)
    return entities[enemy].hp1 <= 0
end
function fireAtEnemy(damage, enemy)
    entities[enemy].hp1 = entities[enemy].hp1 - damage
    --print("fire! hp1=" .. entities[enemy].hp1)
end
function hitEnemy(damage, enemy)
    entities[enemy].hp2 = entities[enemy].hp2 - damage
    --print("hit! hp2=" .. entities[enemy].hp2)
end

function load()
    local spawnX = entity.spawn.x
    local spawnY = entity.spawn.y
    local spawnType = entity.spawn.type

    entity.damage = entity.spawn.damage
    entity.hp1 = entity.spawn.hp
    entity.hp2 = entity.spawn.hp

    entity.timeleft = 0
    entity.rethink = false

    local et = createTransform(entity.index)
    et.x = spawnX
    et.y = spawnY
    et.z = -1
    et.rz = 0
    local drawable = createDrawable(entity.index)
    drawable.mesh = game.tilemap.res.mesh
    drawable.material = newMaterialTexture("sprites.txt")
    setMaterialTextureTile(drawable.material, spawnType.name)
end

function disposeEnemy(enemy)
    deleteEntity(enemy)
end

function calcMove()
    local t = entity.transform
    local fromX = math.floor(t.x + 0.5)
    local fromY = math.floor(t.y + 0.5)
    local dir = game.gamemap:getDirection(fromX, fromY)

    local finished = fromX == entity.spawn.targetX and fromY == entity.spawn.targetY
    if (finished) then
        entity.finished = true
    end
    if (dir == GameMap.PathDirections.unknown or finished) then
        entity.timeleft = 0
        return
    end

    local toX = fromX + dir.x
    local toY = fromY + dir.y
    local progress = 0
    if (dir.align == 'x') then
        progress = (t.x - fromX) / (toX - fromX)
    else
        progress = (t.y - fromY) / (toY - fromY)
    end

    entity.move = {
        fromX = fromX,
        toX = toX,
        fromY = fromY,
        toY = toY,
        dir = dir,
        progress = progress
    }
end

function update()
    if(game.finished) then
        return
    end

    local t = entity.transform

    if (entity.rethink) then
        if (entity.move) then
            if (entity.move.dir == game.gamemap:getDirection(entity.move.fromX, entity.move.fromY)) then
            else
                calcMove()
            end
        else
            calcMove()
        end
    end
    entity.rethink = false

    if (entity.move == nil) then
        calcMove()
    end
    entity.timeleft = entity.timeleft + dt
    while (entity.move and entity.timeleft > 0) do
        entity.move.progress = entity.move.progress + entity.timeleft * entity.spawn.type.speed
        entity.timeleft = 0
        if (entity.move.progress > 1) then
            if (entity.move.dir.align == 'x') then
                t.x = entity.move.toX
            else
                t.y = entity.move.toY
            end
            entity.timeleft = (entity.move.progress - 1) / entity.spawn.type.speed
            entity.move = nil
            calcMove()
        end
    end

    if (entity.move) then
        if (entity.move.dir.align == 'x') then
            t.x = entity.move.toX * entity.move.progress + entity.move.fromX * (1 - entity.move.progress)
        else
            t.y = entity.move.toY * entity.move.progress + entity.move.fromY * (1 - entity.move.progress)
        end
        t.rz = entity.move.dir.r
    end

end
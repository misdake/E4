function load()
    local spawn = entity.spawn
    local spawnX = spawn.x
    local spawnY = spawn.y
    local spawnBaseType = spawn.baseType
    local spawnTurretType = spawn.turretType

    local transform = createTransform(entity.index)
    transform.x = spawnX
    transform.y = spawnY
    transform.z = -2

    local base = newEntityParent(entity.index)
    local bt = createTransform(base)
    bt.z = -0.1
    local bd = createDrawable(base)
    bd.mesh = game.tilemap.res.mesh
    bd.material = newMaterialTexture("sprites.txt")
    setMaterialTextureTile(bd.material, spawnBaseType)

    local turret = newEntityParent(entity.index)
    local tt = createTransform(turret)
    tt.z = -0.2
    local td = createDrawable(turret)
    td.mesh = game.tilemap.res.mesh
    td.material = newMaterialTexture("sprites.txt")
    setMaterialTextureTile(td.material, spawnTurretType)

    entity.children = {
        base = base,
        turret = turret
    }

end

function checkInRange(enemy)
    local x1 = entity.transform.x
    local y1 = entity.transform.y
    local x2 = entities[enemy].transform.x
    local y2 = entities[enemy].transform.y
    local range = entity.spawn.range
    local r2 = range * range;
    local dx = x1 - x2
    local dy = y1 - y2
    local dd = dx * dx + dy * dy
    return (dd <= r2)
end

function findEnemy()
    local selected
    for _, enemy in pairs(game.enemies) do
        if (checkInRange(enemy)) then
            selected = enemy
        end
    end
    return selected
end

function fire(enemy)

end

function lookat(x, y)
    local angle = math.atan(y - entity.spawn.y, x - entity.spawn.x) + math.pi / 2
    local tt = entities[entity.children.turret].transform
    tt.x = math.sin(angle) * 0.17
    tt.y = -math.cos(angle) * 0.17
    tt.rz = angle
end

function update()
    --check current enemy in range
    if (entity.selectedEnemy ~= nil) then
        if (not checkInRange(entity.selectedEnemy)) then
            entity.selectedEnemy = nil
        end
    end

    --find enemy if no current selected
    if (entity.selectedEnemy == nil) then
        entity.selectedEnemy = findEnemy()
    end

    --lookat this enemy
    if (entity.selectedEnemy ~= nil) then
        local et = entities[entity.selectedEnemy].transform
        lookat(et.x, et.y)
    end
end
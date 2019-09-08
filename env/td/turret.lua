function load()
    local spawn = entity.spawn
    local spawnX = spawn.x
    local spawnY = spawn.y
    local reloadTime = spawn.reloadTime
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
        turret = turret,
        bullet = nil,
    }
    entity.reloadTime = reloadTime
    entity.reloadNext = 0

    lookatAngle(0)
end

function spawnBullet()
    local spawnBulletType = entity.spawn.bulletType

    local bullet = newEntityParent(game.rootIndex)
    local lt = createTransform(bullet)
    lt.z = -1.8
    local ld = createDrawable(bullet)
    ld.mesh = game.tilemap.res.mesh
    ld.material = newMaterialTexture("sprites.txt")
    setMaterialTextureTile(ld.material, spawnBulletType)

    entity.children.bullet = bullet
    lookatAngle_bullet()
end

function checkBullet()
    if (entity.children.bullet == nil) then
        entity.reloadNext = entity.reloadNext - dt
        if (entity.reloadNext <= 0) then
            entity.reloadNext = 0
            spawnBullet()
            --print("spawn bullet")
        end
    end

    if (entity.children.bullet ~= nil) then
        if (entity.reloadNext <= 0 and entity.selectedEnemy ~= nil) then
            fireBullet(entity.selectedEnemy)
            --print("fire bullet")
        end
    end
end

function fireBullet(enemy)
    createScript(entity.children.bullet, "bullet.lua")
    local spawn = {
        speed = 5,
        damage = 1,
        enemy = enemy,
    }
    entities[entity.children.bullet].spawn = spawn

    fireAtEnemy(spawn.damage, spawn.enemy)

    entity.children.bullet = nil
    entity.reloadNext = entity.reloadTime
end

function disposeTurret(turret)
    local children = entities[turret].children
    if (children.base ~= nil) then
        deleteEntity(children.base)
    end
    if (children.turret ~= nil) then
        deleteEntity(children.turret)
    end
    if (children.bullet ~= nil) then
        deleteEntity(children.bullet)
    end
    deleteEntity(turret)
end

function checkInRange(enemy)
    if (entities[enemy].transform == nil) then
        return false
    end

    if (enemyWillDie(enemy)) then
        return false
    end

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
    for enemy, valid in pairs(game.enemies) do
        if (valid) then
            if (checkInRange(enemy)) then
                selected = enemy
            end
        end
    end
    return selected
end

function lookat(x, y)
    local angle = math.atan(y - entity.spawn.y, x - entity.spawn.x) + math.pi / 2
    lookatAngle(angle)
end
function lookatAngle_bullet()
    local angle = entity.angle
    if (entity.children.bullet ~= nil) then
        local lt = entities[entity.children.bullet].transform
        lt.x = math.sin(angle) * 0.5 + entity.spawn.x
        lt.y = -math.cos(angle) * 0.5 + entity.spawn.y
    end
end
function lookatAngle(angle)
    entity.angle = angle
    local tt = entities[entity.children.turret].transform
    tt.x = math.sin(angle) * 0.17
    tt.y = -math.cos(angle) * 0.17
    tt.rz = angle
    lookatAngle_bullet()
end

function update()
    if(game.finished) then
        return
    end

    if (entity.selectedEnemy ~= nil) then
        if (not game.enemies[entity.selectedEnemy]) then
            entity.selectedEnemy = nil
        end
    end

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
    checkBullet()
end
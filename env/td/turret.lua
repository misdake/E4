function load()
    local spawn = entity.spawn
    local spawnX = spawn.x
    local spawnY = spawn.y
    local spawnBaseType = spawn.baseType
    local spawnTurretType = spawn.turretType

    local transfrom = createTransform(entity.index)
    transfrom.x = spawnX
    transfrom.y = spawnY
    transfrom.z = -2

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

function findEnemy()

end

function fire(enemy)

end

function lookat(x, y)
    local angle = math.atan(y - entity.spawn.y, x - entity.spawn.y) + math.pi / 2
    local tt = entities[entity.children.turret].transform
    tt.x = math.sin(angle) * 0.17
    tt.y = -math.cos(angle) * 0.17
    tt.rz = angle
end

function update()
    --lookat mouse position
    --TODO extract this xy calculation
    --local t = entities[game.rootIndex].transform
    --local mx = inputStateCurr.mouseX / screenWidth * 2 - 1
    --local my = 1 - inputStateCurr.mouseY / screenHeight * 2
    --local x = (mx - t.x) / t.sx
    --local y = (my - t.y) / t.sy
    --lookat(x, y)

    --lookat this enemy
    local enemy
    for _, e in pairs(game.enemies) do
        enemy = e
    end
    local et = entities[enemy].transform
    lookat(et.x, et.y)
end
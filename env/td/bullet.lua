function load()
    entity.enemy = entity.spawn.enemy
    entity.speed = entity.spawn.speed
    entity.damage = entity.spawn.damage
end

function update()
    --if this bullet is done, wait for destruction
    if (entity.finished) then
        entity.drawable.visible = false
        game.entitiesToDelete[entity.index] = true
        return
    end

    --check enemy is invalid
    if (not game.enemies[entity.enemy]) then
        entity.finished = true
        return
    end

    --move towards enemy
    local from = entity.transform
    local to = entities[entity.enemy].transform
    local length = entity.spawn.speed * dt

    local dx = from.x - to.x
    local dy = from.y - to.y
    local d = math.sqrt(dx * dx + dy * dy)
    if (d < length) then
        entity.finished = true
        hitEnemy(entity.damage, entity.enemy)
    else
        from.x = from.x - dx * length / d
        from.y = from.y - dy * length / d
    end

end
function load()
	print("load "..entity.id)
end

function update()
	-- print("update "..entity.id)
    entity.transform.ry = math.fmod(entity.transform.ry + 3 * dt, math.pi * 2)
end
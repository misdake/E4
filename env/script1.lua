function load()
	print("load "..entity.id)
end

function update()
	-- print("update "..entity.id)
	requestTransform(entity.id)
    entity.transform.ry = math.fmod(entity.transform.ry + 0.05, math.pi * 2)
end
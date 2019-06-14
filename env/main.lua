function load()
	print("load "..entity.id)

    local e = createEntity()
    local transform = createTransform(e)
    transform.x = 0.1;
    transform.y = -0.5;
    transform.z = -0.1;
    local drawable = createDrawable(e)
    drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture("footprint.png")
    createScript(e, "script1.lua")

end
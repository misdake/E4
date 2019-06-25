function mouse1()
	return (not inputStatePrev.mouseButton1) and inputStateCurr.mouseButton1
end
function mouse3()
	return (not inputStatePrev.mouseButton3) and inputStateCurr.mouseButton3
end

function createChild(x, y, size, textureName)
    local e = newEntity()
    local transform = createTransform(e)
    transform.x = x;
    transform.y = y;
    transform.z = 0;
    transform.sx = size;
    transform.sy = size;
    transform.parent = entity.index;
    local drawable = createDrawable(e)
    drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture(textureName)
    return e
end

function gameLoad()
    entity.birdsize = 0.08
    entity.acc = -5
    entity.pipeinterval = 1.4
    entity.pipegap = 0.33
end

function newY()
	return math.random() - 0.5
end

function createPipe(index)
    entity.pipes[index] = {
        pipe1 = createChild(0, 0, 1, "pipe.png"),
        pipe2 = createChild(0.5, 0, 1, "pipe.png"),
        x = 0,
        y = 0
    }
    print("createPipe "..index)
end

function setupPipe(index)
    entity.pipes[index].x = 1 + index * entity.pipeinterval
    entity.pipes[index].y = newY()
end

function gameStart()
    entity.running = true
    entity.x = -0.5
    entity.y = 0.5
    entity.speed = 0
    setupPipe(0)
    setupPipe(1)
    setupPipe(2)
end
function gameEnd()
    entity.running = false
end

function updatePipe(index)
    local pipe = entity.pipes[index]
    local t1 = entities[pipe.pipe1].transform
    local t2 = entities[pipe.pipe2].transform
    t1.x = pipe.x
    t2.x = pipe.x
    t1.y = pipe.y - 1 - entity.pipegap
    t2.y = pipe.y + 1 + entity.pipegap
    t1.sx = 0.5
    t2.sx = 0.5
    t1.sy = 2
    t2.sy = -2

    if(entity.running) then
        pipe.x = pipe.x - dt * 1
    end

    if (pipe.x < -screenWidth /screenHeight - 0.5) then
        pipe.x = pipe.x + 3 * entity.pipeinterval
        pipe.y = newY()
    end
end

function load()
    gameLoad()
    -- entity.transform.sx = screenHeight / screenWidth
    entity.bird = createChild(-0.5, 0.5, 0.25, "flappybird.png")

    entity.pipes = {}
    createPipe(0)
    createPipe(1)
    createPipe(2)

    gameStart()
end

function checkDead()
    if (entity.y < -1) or (entity.y > 1) then
        gameEnd()
    end

    local x = entity.x
    local y = entity.y
    for i = 0,2 do
    	local pipe = entity.pipes[i]
    	local left = pipe.x - 0.25
    	local right = pipe.x + 0.25
    	local dy = entity.birdsize
    	if (x <= left) and (x > left - entity.birdsize) then
    		local dx = left - x
    		dy = math.sqrt(entity.birdsize * entity.birdsize - dx * dx)
    	end
    	if (x >= right) and (x < right + entity.birdsize) then
    		local dx = x - right
    		dy = math.sqrt(entity.birdsize * entity.birdsize - dx * dx)
    	end
    	if ((x > left - entity.birdsize) and (x < right + entity.birdsize)) then
    		local up = pipe.y + entity.pipegap - dy
    		local down = pipe.y - entity.pipegap + dy
    		if (y < down) or (y > up) then
        		gameEnd()
    		end
    	end
    end

end

function update()
    updatePipe(0)
    updatePipe(1)
    updatePipe(2)

    local birdTransform = entities[entity.bird].transform

    if (entity.running) then
	    if mouse1() then
	        math.randomseed(math.random() + entity.speed)
	        entity.speed = 2
	    end
	    entity.speed = entity.speed + dt * entity.acc
	    entity.y = entity.y + entity.speed * dt
	    birdTransform.x = entity.x
	    birdTransform.y = entity.y

	    birdTransform.rz = math.atan(entity.speed / 3)

	    checkDead()
	end

    if (not entity.running) and mouse3() then
        gameStart()
    end
end
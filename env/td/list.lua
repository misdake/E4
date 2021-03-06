List = { first = 0, last = -1 }
function List:new()
    local o = {}
    setmetatable(o, self)
    self.__index = self
    return o
end
function List:isEmpty()
    return self.first > self.last
end
function List:pushleft (value)
    local first = self.first - 1
    self.first = first
    self[first] = value
end
function List:pushright(value)
    local last = self.last + 1
    self.last = last
    self[last] = value
end
function List:getleft()
    local first = self.first
    if first > self.last then
        error("self is empty")
    end
    local value = self[first]
    return value
end
function List:popleft()
    local first = self.first
    if first > self.last then
        error("self is empty")
    end
    local value = self[first]
    self[first] = nil        -- to allow garbage collection
    self.first = first + 1
    return value
end
function List:getright()
    local last = self.last
    if self.first > last then
        error("self is empty")
    end
    local value = self[last]
    return value
end
function List:popright()
    local last = self.last
    if self.first > last then
        error("self is empty")
    end
    local value = self[last]
    self[last] = nil         -- to allow garbage collection
    self.last = last - 1
    return value
end
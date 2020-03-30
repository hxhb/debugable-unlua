--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"
-- if WITH_LUA_DEBUG then
--     require("LuaPanda").start("127.0.0.1",8818)
-- end

local TemplateName = Class()

--function TemplateName:Initialize(Initializer)
--end

--function TemplateName:UserConstructionScript()
--end

function TemplateName:ReceiveBeginPlay()
	self.Overridden.ReceiveBeginPlay(self)	
end

--function TemplateName:ReceiveEndPlay()
--	self.Overridden.ReceiveEndPlay(self)	
--end

-- function TemplateName:ReceiveTick(DeltaSeconds)
--	self.Overridden.ReceiveTick(self,DeltaSeconds)	
-- end

--function TemplateName:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
--end

--function TemplateName:ReceiveActorBeginOverlap(OtherActor)
--end

--function TemplateName:ReceiveActorEndOverlap(OtherActor)
--end

return TemplateName

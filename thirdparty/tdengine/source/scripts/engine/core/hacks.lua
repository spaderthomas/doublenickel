local hacks = [[
WEIRD_COLLIDER_CONSTRUCTION
I want a Collider, but I don't want it to be attached to an entity. Unfortunately, because
of some poor choices, all the component classes get their virtual methods assigned at instantiation,
not where the instance is defined. So, I hack around this by instantiating the component in the
usual code path.

Better would be to make tdengine.component() which assigns directly to the class


ORPHAN_COLLIDERS
Some colliders aren't attached to an entity, which makes this code a little weird



SCROLL_VISUAL_INTERP
Also just for the dialogue box. I don't want the scroller to jump when it starts the interpolation,
so I have this override that says "just draw it at the bottom"
]]

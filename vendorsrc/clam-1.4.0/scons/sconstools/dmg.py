import os

def create_dmg( target, source, env) :
	volume = str(target[0]).replace('.dmg', '')

	os.system( "mkdir DMG" )
	for s in source :
		os.system( "cp -r %s DMG"%s )
	os.system( "hdiutil create -srcfolder DMG -volname %s %s"%(volume, target[0]) )
	#os.system( "sudo hdiutil create -srcfolder DMG -volname %s -uid 0 %s"%(volume, target[0]) )
	os.system( "rm -rf DMG" )

def create_dmg_message( target, source, env):
	return "Creating DMG package"

def generate(env) :
	"""Add Builders and construction variables for qt to an Environment."""
	print "Lodading dmg tool..."
	env.Append( BUILDERS={'Dmg' : 
			env.Builder( action=env.Action(create_dmg, create_dmg_message ))
		} )

def exists(env) :
	return True


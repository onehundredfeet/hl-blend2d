package ;



#if eval
class Generator {

	// Put any necessary includes in this string and they will be added to the generated files
	static var INCLUDE = "
#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#include <hl.h>
#include \"hl-blend2d.h\"
";
	
	public static function generateCpp() {	
		var options = { idlFile : "blend2d/blend2d.idl", nativeLib : "blend2d", outputDir : "src", includeCode : INCLUDE, autoGC : true };
		webidl.Generate.generateCpp(options);
	}

}
#end
echo y|del "..\..\..\SDK\LBBML_ROOT\include\*.*"
echo y|del "..\..\..\SDK\LBBML_ROOT\lib\*.*"
echo y|del "..\..\..\SDK\LBBML_ROOT\bin\*.*"

copy "LBBML\Common\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\Common\"

copy "LBBML\FileSystem\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\FileSystem\"
copy "LBBML\FileSystem\Config\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\FileSystem\Config\"

copy "LBBML\DataSimulator\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\DataSimulator\"
copy "LBBML\DataSimulator\Distances\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\DataSimulator\Distances\"
copy "LBBML\DataSimulator\Functions\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\DataSimulator\Functions\"

copy "LBBML\MathWorker\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\MathWorker\"

copy "LBBML\StatsCollector\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBML\StatsCollector\"

copy "LBBMLGraphics\GraphicsCommon\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphics\GraphicsCommon\"
copy "LBBMLGraphics\Loader\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphics\Loader\"
copy "LBBMLGraphics\BoundingBox\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphics\BoundingBox\"
copy "LBBMLGraphics\Drawable\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphics\Drawable\"
copy "LBBMLGraphics\LightSource\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphics\LightSource\"
copy "LBBMLGraphics\Viewer\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphics\Viewer\"
copy "LBBMLGraphics\Stats\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphics\Stats\"

copy "LBBMLGraphicsApp\SoftShadowMapping\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphicsApp\SoftShadowMapping\"
copy "LBBMLGraphicsApp\SoftShadowMapping\Render\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphicsApp\SoftShadowMapping\Render"
copy "LBBMLGraphicsApp\SplitScreenRendering\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphicsApp\SplitScreenRendering\"
copy "LBBMLGraphicsApp\LBBMLGraphicsAppCommon\*.h" "..\..\..\SDK\LBBML_ROOT\include\LBBMLGraphicsApp\LBBMLGraphicsAppCommon\"

copy "Debug\*.lib" "..\..\..\SDK\LBBML_ROOT\lib\"
copy "Debug\*.dll" "..\..\..\SDK\LBBML_ROOT\bin\"

copy "Release\*.lib" "..\..\..\SDK\LBBML_ROOT\lib\"
copy "Release\*.dll" "..\..\..\SDK\LBBML_ROOT\bin\"

pause

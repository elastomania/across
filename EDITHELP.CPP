#include	"all.h"

//#define UCSOSOROK "", "You can dismiss this dialog by pressing ENTER, ESC, or clicking the right mouse button." );
#define UCSOSOROK "" );

void edithelp( void ) {
	dialog(
"GENERAL HELP",

#ifdef SARVARI
"This shareware version of the editor has severe limitations, it is included mainly to",
"give you a preview of the full editor that comes with the registered version. You can",
"make only a maximum of 3 polygons on a level with only a maximum of ten vertices per",
"polygon. The maximum number of objects (food and killer) is 6. In the registered",
"version these numbers are: maximum 300 polygons, maximum 1000 vertices per polygon",
"(but no more than 5000 altogether), maximum 50 objects.",
"",
"USAGE",
#endif

"On the left of the screen is a column of buttons. The buttons in the upper half are",
"command buttons, that means they will take action at the moment you click on them with",
"the left mouse button. The buttons in the lower half are the tool selection buttons.",
"If you click on them with the left mouse button, you select a tool. You can use a tool",
"in the working area of the screen. A one line help of the current tool is always displayed",
"at the top of your screen.",
"",
"You can get help on the command and tool selection buttons by clicking on any of them",
"with the right mouse button.",
/*"",
"If you create a new level, two objects will be already present. The start object and the",
"exit object (with an 'S' and an 'E' inside them). The start object determines the place",
"of the left wheel when you start to play on a particular level.", */
UCSOSOROK
}

void menuhelp_exit( void ) {
	dialog(
//"EXIT HELP SCREEN",
"With the Exit button you can exit the editor.",
"If there are any unsaved changes in the level file in the editor, a dialog will",
"appear asking if you really want to exit without saving these changes.",
UCSOSOROK
}

void menuhelp_zoomout( void ) {
	dialog(
//"ZOOM OUT HELP SCREEN",
"With the Zoom Out button you can make larger the view window of the editor.",
"In the upper right corner you can always see the actual zoom value. If this value is 1",
"you cannot make the zoom window any larger.",
UCSOSOROK
}

void menuhelp_save( void ) {
	dialog(
//"SAVE HELP SCREEN",
"With the Save button you can save the content of the editor to a level file.",
"The name of the level file is always displayed at the top of the screen.",
"If you haven't named your file, you will be prompted for a file name.",
"In this case you must enter the name without an extension and a path, as the '.LEV'",
"extension will be automatically added and you can save the file only in your current",
"directory.",
UCSOSOROK
}

void menuhelp_save_as( void ) {
	dialog(
//"SAVE AS HELP SCREEN",
"With the Save As button you can save the content of the editor to a level file",
"with a new name.",
"You will be prompted for a file name. You must enter the name without an extension and",
"a path, as the '.LEV' extension will be automatically added and you can save the file",
"only in your current directory.",
"If you click on the Save Locked button, you can lock the saved file. This means you",
"cannot load it into the editor any more. You should do this if you are ready with a",
"level, or when you give your level file to somebody and do not want he/she to modify",
"your original file. You can read more about this in EDITOR.TXT!",
UCSOSOROK
}

void menuhelp_open( void ) {
	dialog(
//"OPEN HELP SCREEN",
"With the Open button you can open a level file in the editor.",
"A dialog will appear with a list of the level files (all files with .LEV extension) in",
"the current directory. You can pick the desired file with one mouse click on the name of",
"the file or by pressing ENTER when the name of the file is highlighted.",
"You can use the two long buttons at the top and the bottom to go up and down in the list.",
"You can use the Up, Down, PageUp, PageDown and Esc keys also."
"",
"If you want to open a new file with a new name, you should choose the New button instead",
"of the Open button, then use the Save As button to give a name to your file.",
UCSOSOROK
}

void menuhelp_new( void ) {
	dialog(
//"NEW HELP SCREEN",
"With the New button you can start to edit a new level file in the editor.",
UCSOSOROK
}

void menuhelp_play( void ) {
	dialog(
//"SAVE AND PLAY HELP SCREEN",
"With the Save and Play button you can try out the level that you are currently editing.",
"You can press the P shortcut key also to try your level.",
"The content of the editor will be saved to disk before you can play.",
UCSOSOROK
}

void menuhelp_zoom_fill( void ) {
	dialog(
//"ZOOM FILL HELP SCREEN",
"With the Zoom Fill button you can make the view window fit the entire content of",
"the editor.",
UCSOSOROK
}

void menuhelp_check( void ) {
	dialog(
//"CHECK HELP SCREEN",
"With the Check Topology button you can check the level you are editing for any",
"basic errors. Before you want to try a level, you may want to check it first.",
"When check finds the first error, it will display a dialog box with an error message,",
"then it will zoom in onto the exact location of the error.",
"Until you correct all errors you cannot play on the level,"
"though you can save it to a file.",
UCSOSOROK
}

void menuhelp_level_name( void ) {
	dialog(
//"LEVEL NAME HELP SCREEN",
"With the Level Name button you can give a name to the level you are editing.",
UCSOSOROK
}

// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK
// TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK TOOLOK

void menuhelp_move( void ) {
	dialog(
//"MOVE TOOL HELP SCREEN",
"With the Move tool you can move vertices and objects.",
"Click the left mouse button near a vertex or the center of an object to grab it.",
"Move the cursor to the location you want to move the vertex or the object to",
"and click the left mouse button.",
"You can press ESC or click the right mouse button to cancel the operation.",
UCSOSOROK
}

void menuhelp_zoom_in( void ) {
	dialog(
//"ZOOM IN TOOL HELP SCREEN",
"With the Zoom In tool you can zoom the view window into any part of the window",
"you see at the moment. Click the left mouse button to place the first corner of",
"the window, then click again to place the second corner.",
"You can press ESC or click the right mouse button to cancel the operation.",
UCSOSOROK
}

void menuhelp_create_vertex( void ) {
	dialog(
//"CREATE VERTEX TOOL HELP SCREEN",
"With the Create Vertex tool you can create new vertices. If you click the left",
"mouse button near an existing vertex, you will add vertices to an existing polygon.",
"If you click far from any vertices, you will create the first vertex of a new polygon.",
"If you want to add to an existing polygon, but find that when you click near a vertex",
"you would like to add vertices to the opposite way from this point, you should press",
"SPACE. Try this function to see exactly how it works. Similarly you can press ENTER to",
"change the direction you are adding the vertices. Please take some moments to",
"experiment with these two methods, because it is important to understand the way",
"they work.",
"You should press ESC or click the right mouse button when you have finished",
"adding vertices. In this case the vertex currently grabbed will be deleted.",
UCSOSOROK
}

void menuhelp_delete_vertex( void ) {
	dialog(
//"DELETE VERTEX TOOL HELP SCREEN",
"With the Delete Vertex tool you can delete vertices. Just click the left mouse",
"button near a vertex and it will disappear. You cannot delete any vertices of a",
"polygon if it has only three vertices. To delete the entire polygon, use",
"the Delete Polygon tool.",
UCSOSOROK
}


void menuhelp_delete_polygon( void ) {
	dialog(
//"DELETE POLYGON TOOL HELP SCREEN",
"With the Delete Polygon tool you can delete polygons. Just click the left mouse",
"button near a vertex of a polygon and the polygon will disappear. You cannot",
"delete all the polygons, at least one polygon must exist at any given time in",
"the editor."
UCSOSOROK
}


void menuhelp_create_food( void ) {
	dialog(
//"CREATE FOOD TOOL HELP SCREEN",
"With the Create Food tool you can create new food objects. Just click the left",
"mouse button to place a new food object. You can see an 'F' inside the food objects.",
UCSOSOROK
}


void menuhelp_create_killer( void ) {
	dialog(
//"CREATE KILLER TOOL HELP SCREEN",
"With the Create Killer tool you can create new killer objects. Just click the left",
"mouse button to place a new killer object. You can see a 'K' inside the killer objects.",
UCSOSOROK
}


void menuhelp_delete_object( void ) {
	dialog(
//"DELETE OBJECT TOOL HELP SCREEN",
"With the Delete Object tool you can delete food and killer objects. Just click the left",
"mouse button near the center of an object and it will disappear.",
"You can only delete the food and killer objects, which have either an 'F' or 'K' inside.",
"You cannot delete the start and exit objects, which has an 'S' or 'E' inside.",
UCSOSOROK
}









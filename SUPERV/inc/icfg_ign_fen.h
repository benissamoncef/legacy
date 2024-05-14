#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <rtworks/ipc_proto.h>


#include "std.h"         /* <stdio.h> etc., scalar & macro definitions */
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>


#include <Mrm/MrmAppl.h>
#include <Mrm/MrmPublic.h>


#include <Xm/XmStrDefs.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrollBar.h>
#include <Xm/Separator.h>
#include <Xm/MessageB.h>

#include "dvstd.h"
#include "dvtools.h"
#include "dvGR.h"
#include "VOstd.h"
#include "dvparams.h"


#include "Tfundecl.h"    /* T routines (screens, drawports & views) */
#include "VOfundecl.h"   /* VO routines (objects) */
#include "VUerfundecl.h" /* VUer routines (event handling routines) */
#include "VGfundecl.h"   /* VG routines (data grp and var descriptor routines)
 */
#include "VTfundecl.h"   /* VT routines */
#include "VUfundecl.h"   /* VU routines */
#include "VPfundecl.h"   /* VU routines */

#include "xdc.h"
#include "xdl.h"
#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsc.h"
#include "xzsa.h"
#include "xzse.h"
#include "xzsp.h"
#include "xzsm.h"
#include "xzss.h"
#include "xzga.h"




#define cg_nb_hierarchies       5


typedef struct s_InfosMenu {
	Widget                  w_menu_fen;
	Widget                  w_menu_bar;
	int                     nb_widget;
	Widget                  tab_w [200];
} tg_InfosMenu;






int                      vg_NumScreens;
tg_InfosMenu*            vg_PtrSurMenuACreer;
tg_InfosMenu vg_MENU[5];             /* widget menu d'administration */

Widget vg_GRSYNT[5];           /* widget Affichage Synthese */

Widget MENU = (Widget) NULL;
Widget MENU_MB = (Widget) NULL;
Widget GRMACH_SYNT = (Widget) NULL;
Widget GRMACH_SYNT_FM = (Widget) NULL;
Widget GRMACH_SYNT_SCROLL = (Widget) NULL;
Widget GRMACH_SYNT_DA = (Widget) NULL;


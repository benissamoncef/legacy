/*E*/
/* Fichier : $Id: scmd_root.c,v 1.3 2011/01/10 19:07:22 gesconf Exp $	 Release : $Revision: 1.3 $        Date : $Date: 2011/01/10 19:07:22 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SCMD * FICHIER scmd_root.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module permet l'execution d'une commande root poar sapl.x
* la commande est donnee en argument: 
*  exmemple:
*    scmd.x "cd /; /etc/shutdown -h"
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 13 Oct 1994     : Creation
* JPL	11/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.3
*******************************************************/
 
#include	<stdlib.h>
#include	<unistd.h>


 static char *version = "$Id: scmd_root.c,v 1.3 2011/01/10 19:07:22 gesconf Exp $ $Revision: 1.3 $ $Date: 2011/01/10 19:07:22 $ : SCMD" ;


main(int argc, char *argv[])
{
  int vl_cr=0;
  setuid(0);
  vl_cr=system(argv[1]);
  exit (vl_cr);
}

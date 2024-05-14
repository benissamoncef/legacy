/*E*/
/* Fichier : $Id: elnt_dir.c,v 1.7 2011/02/25 11:17:08 gesconf Exp $        Release : $Revision: 1.7 $        Date : $Date: 2011/02/25 11:17:08 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELNT * FICHIER elnt_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'envoi du message mode lent >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	10 Fev 1995	: Creation
* Volcic.F	version 1.2	17 Mar 1995	: Divers 
* Volcic.F	version 1.3	07 Avr 1995	: Modification : creation executable 
* Volcic.F	version 1.4	07 Jun 1995	: Modification : creation executable 
* Mismer.D	version 1.5	07 Nov 1995	: Ajout tempo attente 5s avant fin de tache (DEM/xxxx) 
* JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "elnt_dir.h"

/* definitions de constantes */

#define CM_DEBUT	"-DEBUT"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: elnt_dir.c,v 1.7 2011/02/25 11:17:08 gesconf Exp $      : elen_dir.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Programme principal
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int main( argc, argv )

/*
* ARGUMENTS EN ENTREE :
*   argc : Nombre d'arguments
*   argv : liste des arguments
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION
*
* Initialisations :connexion rtserver, envoi du message mode lent
*
------------------------------------------------------*/

int     argc;
char    **argv;
{

	XDY_Entier              vl_retcode = 0;
	XDY_Entier              vl_i = 0;
        XDY_NomMachine		pl_NomMachine = "";
        XDY_Booleen		vl_Etat = XDC_FAUX;

      /*A
      *  initialisation semaphore
      */
      sem_init(&vg_semaphore, 0, 1);

        /*A Connexion au RTserver et initialisation des variables */
        if( XZSC_01InitCnxSup(  NULL,
                                argv[ 0 ],
                                XZSCC_INTERDIRE,
                                XZSCC_AUTORISER,
                                XZSCC_INTERDIRE,
                                NULL,
                                "/dev/null" ) != XDC_OK )
        {
           /*B Affichage Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion avec le RTserver." );
           exit( XDC_NOK );
        }
        else
        {
           /*B Affichage Trace */
           XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
        }

        /*A Recuperation du nom de la machine et du debut ou fin de lancement du mode lent */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "Nb Arg = %d", argc);
        if ( argc == 3 )
        {
           /*A Recuperation du nom de la machine */
           strcpy(pl_NomMachine,argv[2]);
           for ( vl_i = 0; vl_i < strlen ( pl_NomMachine ); vl_i ++ )
              pl_NomMachine[vl_i] = toupper ( pl_NomMachine[vl_i] );
        }
        else
	   XZSC_07NomMachine ( pl_NomMachine );
		
        for ( vl_i = 0; vl_i < strlen ( argv[1] ); vl_i ++ )
              argv[1][vl_i] = toupper ( argv[1][vl_i] );
        vl_Etat = ( !strncmp ( argv[1], CM_DEBUT, 2 ) ) ? XDC_VRAI : XDC_FAUX;
        XZST_03EcritureTrace( XZSTC_DEBUG1, "NomMach = %s\tEtat = %d", pl_NomMachine, vl_Etat);
        
	/*A Envoi du message a la machine concernee */
	if ( ( vl_retcode = el_Mode_Lent ( pl_NomMachine, vl_Etat ) ) != XDC_OK )
		XZST_10ArchiverMsgSyst(0,"elnt_dir.c : Message XDM_MODE_LENT non effectue", NULL);    

	/*  Attente de 5s avant deconnexion au RTserver sinon message transmis 1 fois sur 5 */
        sleep ( 5 );
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* Envoi du Message de fonctionnement en mode lent ou non
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int el_Mode_Lent ( XDY_NomMachine	pa_Machine,
		   XDY_Booleen		va_Mode_Lent )
/*
* ARGUMENTS EN ENTREE :
*
* XDY_NomMachine	pa_Machine	: Nom de la Machine en Mode Lent
* XDY_Boolee		va_Mode_Lent	: Mode_Lent Active ou Desactive
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* XDC_OK
* XDC_NOK
* 
* CONDITION D'UTILISATION
*
* FONCTION
* 
* Envoi du Message de fonctionnement en mode lent ou non
*
------------------------------------------------------*/
{
 static char *version = "$Id: elnt_dir.c,v 1.7 2011/02/25 11:17:08 gesconf Exp $      : el_Mode_Lent" ;
 
 T_INT2			vl_Mode_Lent = XDC_FAUX;
 XDY_Datagroup		pl_DG_Mode_Lent = "";
	
 
 XZST_03EcritureTrace( XZSTC_FONCTION, " IN : %s : Machine = %s\tMode Lent = %d", version, pa_Machine, va_Mode_Lent );
 
 /*B 
 ** Construction du DataGroup XDG_MODE_LENT<NomMachine> 
 */
 
 vl_Mode_Lent = va_Mode_Lent;
 sprintf ( pl_DG_Mode_Lent, "%s_%s", XDG_MODE_LENT, pa_Machine );
   
 /*B
 ** Envoi du message XDM_MODE_LENT en fournissant les valeur recues  
 */
 
 if (!TipcSrvMsgWrite (	pl_DG_Mode_Lent
                    	,XDM_IdentMsg(XDM_MODE_LENT)
                     	,XDM_FLG_SRVMSGWRITE
                     	,T_IPC_FT_INT2, vl_Mode_Lent
			,NULL))
 {
    XZST_03EcritureTrace(XZSTC_WARNING,"%s : Envoi message XDM_MODE_LENT non effectue", version);
    XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : %s ", version);
    return (XDC_NOK);
 }
   
 /*B
 ** Envoi du message 
 */
   
 TipcSrvFlush();

 XZST_03EcritureTrace(XZSTC_INFO,"%s : Envoi message XDM_MODE_LENT effectue avec succes",version);
 XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : %s ", version);
 return (XDC_OK); 		
}

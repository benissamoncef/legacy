/* Fichier : asts_dir.c   Release : 1.1   Date : 05/13/97	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : asts_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache serveur de donnees
* trafic pour le PC STRADA
*************************************************
* HISTORIQUE :
*
* VATHELOT	28 Avr 1997 : Creation 			V1.1
* NIEPCERON	09 Jan 2007 : Ajout attente d'etre a 0+2mn ou 6+2mn ou 12+2mn 
* JPL		17/11/10    : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.5
* SFO           06/10/14 : Adaptation pour compilation sous linux
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
************************************************/

#include <xzsem.h>
#include "asts_dir.h"
#include "xzems.h"

#define OFFSET_6MN             2



/* d�finition des variables globales � l'application */


/* definition des variables locales au module */
static char *	version = "@(#)asts_dir.c	1.1 05/13/97 : commentaire" ;


/* declaration de fonctions internes */
void asts_sortir();

/* definition de fonctions externes */


int main(int argc, char *argv[])
{
int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr	= XDC_OK;
T_IPC_MSG	pl_Msg;
XDY_NomSite	pl_NomSite;
int		vl_ValRet;
int			vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee1, vl_annee2;
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;


	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/* Connexion au RTServer */
	vl_cr = XZSC_01InitCnxSup(argv[1]
		 ,argv[0]
		 ,XZSCC_INTERDIRE /* mise a jour horodate*/
		 ,XZSCC_AUTORISER /* systeme trace */
		 ,XZSCC_INTERDIRE /* synchro tsetat */
		 ,asts_sortir /* fontion de sortie */
		 ,"/dev/null"     /* redirection sortie standard */
		 ) ;


	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur XZSC_01InitCnxSup cr = %d",vl_cr);
	 	exit;
      	}
      	
      	
      	/*A
        ** Recuperation du site de la machine
        */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, 
                	"%s : Recuperation du Nom du site impossible.",version);
                exit(XDC_NOK);
        }
        XZST_03EcritureTrace( XZSTC_DEBUG1, 
                	"%s : Recuperation du Nom du site %s.",version,pl_NomSite);

	/*A 
	** Connexion a la base 
	*/
	if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
        {
                /*B
                ** Ecriture Trace
                */
                XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion au serveur SQL de %s impossible .",version,pl_NomSite);
                exit;
        }
        XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion au serveur SQL de %s reussie.",version,pl_NomSite);

        /* avant de demarrer dans le programme sans fin, on attend d'etre a 0+2 ou 6+2 ou 12+2 etc...*/
	vl_heure = 1;
	while (vl_minute%6 != OFFSET_6MN )
	{
	  sleep(20);

	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
	XZSM_11ConversionHorodate(vl_HorodateSec, &vl_Horodate);

	  if (sscanf( vl_Horodate, "%2d/%2d/%2d%2d %2d:%2d:%2d",
								&vl_jour,
								&vl_mois,
								&vl_annee1,
								&vl_annee2,
								&vl_heure,
								&vl_minute,
								&vl_seconde) != 7) {
								
					XZST_03EcritureTrace(   XZSTC_WARNING,
							"asts_export_ftp : le format de la date est incorrecte !" );			
            				return;
	                                }

	  XZST_03EcritureTrace(XZSTC_WARNING, "date:%s minute systeme : %d", vl_Horodate, vl_minute );
        } 
        
	XZST_03EcritureTrace( XZSTC_WARNING,
                	"ON A LE BON OFFSET 6MN : ON DEMARRE !!!");
	/* Appel de la fonction d'initialisation */
	vl_cr = asts_export_init();
	
	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur asts_export_init cr = %d",vl_cr);
	 	exit;
      	}

	
	/* Attente bloquante des messages du RTServer */
	while(1)
	{
      		/* Recuperation du msg RTworks suivant */
	 	pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

		if(isMessageNotNULL(pl_Msg))
		{

			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;
         	}
         	
	 	/* erreur RTWORKS */
	 	else 
         	{
			/* Test de Presence RTserver */
			if (vl_IndPre == 0) 
			{
				if (errno)
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
			}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache asts %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				asts_sortir();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void asts_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "asts_sortir  : IN  \n");
	
	/*A
	** Fermeture du fichier trace 
	*/
	
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	}
	
	
	/* Liberation des ressources utilisees dans asts_export.c */
	asts_export_arret();
	
	/*A
	 * Sortie 
	 */
	sprintf( vl_cle , "cle_timer_serveur_radt" );
		
	/* Liberer les ressources du timer */
#ifdef _HPUX_SOURCE
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
#else
	XZSM_02AnnulerProgrammation( vl_cle,(char *) NULL,0);
#endif	
	exit(0);
}

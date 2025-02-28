/*E*/
/* Fichier : drol_dir.c
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : DROL_DIR				*
* FICHIER : drol_dir.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
* PCL   03/06/02        REPERE 1 : traces warning uniquement 1.2
* Hilmarcher 20/11/2003 Modification pour envoi fax depuis PC
*                       DEM 260 (envoi fax GN) 1.3
* JMG	27/04/04	ajout proposition PAL 1.4
* JMG	12/11/04	ajout var globale numero du poste 1.6
*			pilotage PAL en automatique
* PNI	24/02/05	Modification de l'ordre des actions DEM427 1.7
* JMG	19/09/05	plus d appel pour le site DDE 83 
* PNI	21/06/07 : Basculement pour travaux systematiquement en sens inconnu pour traitement 2 sens DEM658
* LCL   09/07/07 : Suppression DY
* PNI	17/07/07 : Basculement pour travaux systematiquement en sens +OFFSET pour traitement 2 sens DEM658
* JPL	01/04/09 : Transmission de l'id. evenement a 'drol_fax_elabore' (DEM 411) 1.14
* JMG	2/10/09 : secto DEM887 1.15
* JMG	3/11/09 : SAD Tunnel DEM895 1.16
* JMG	12/12/10 : abonnement a XDG_LX_IE sur un serveur pour traitement PO Linux 1.17
* ???	xx/02/11 : Au CI, ne pas proposer d'appel pour le secteur CA  1.18
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.19
* JPL	09/01/12 : Changement libelle "message TraficFM" en "message RVA" (DEM 1009)  1.20
* JMG	24/05/12 : linux 1.21
* VR	17/08/12 : Ajout PMVA (DEM/1014 PMA) v1.22
* PNI	14/10/13 : Memorisation de la presence operateur ds secteur v1.24
* JMG 	 18/06/15 :  ordonnancement RVA DEM1130 1.25 1.26
* JPL	03/05/16 : Trace du resultat avec duree de calcul en fichier separe (DEM 1163)  1.27
* JPL	xx/01/17 : Ajout action 'drol_nav_filtre_pal' pour proposer les PAL des BRA (DEM 1193)  1.28
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.29
* LCL	26/10/17 : PRA 1.30
* JPL	09/02/18 : Migration Linux 64 bits (DEM 1274) : Horodate courante obtenue par XZSM pour coherence et portabilite  1.31
* JMG	03/07/18 : SMS RVA LOT25 DEM1301       1.32 
*                                DEM1303 (RVA) 1.33
* RGR	23/10/18 : Correction Synchro revision 1.35
* JMG	2018	: premier message RVA 1.36
* PNI	09/07/19 : Correction premier message RVA en cas d'evt termine 1.37 DEM1318
* LCL	10/09/19 : Pas d'envoi de message RVA si FMC EXERCICE ou ESSAI DEM1349 1.38
* PNI	04/09/20 : Ajout du mecanisme KeppAlive DEM1331 1.39 1.40 1.41 1.42
*			EMS
* JPL	15/12/20 : Ecriture texte message RVA: taille zone tampon  1.43
* AMI 28/01/21 : SAE_191: chantier date: encapsulation localtime pour hiniber TZ car nbseconde deja en TZ
* ABE	10/05/21 : Ajout SONO DEM-SAE244 1.53
* ABE	02/06/21 : Ajout IMU DEM-SAE155 1.55
* JPL	17/09/21 : Extension envoi automatique 1er message RVA (SAE_74)  1.56
* JPL	25/10/21 : Annulation Extension envoi automatique 1er message RVA (SAE_342)  1.57
* PNI   19/07/22 : initialisation de vl_niveau  SAE 396
* PNI   22/11/22 : Retirer le calcul message TFM pour evt terminé SAE 434
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
* LCL 	27/09/23 : Adaptations CNA DEM-483
* LCL	04/10/23 : Adaptations CNA DEM-483 : lecture site principal en base
* GGY	04/12/23 : Ajout de l'appel a drol_pic_filtre dans drol_dir_cb + ajout vg_ordre_pic (DEM483)
*************************************************/
#include "xzems.h"
#include <xzsem.h>
#include "xdg.h"
#include "drol_dir.h"

#include "drol_nav.h"
#include "drol_pal.h"
#include "drol_bad.h"
#include "drol_baf.h"
#include "drol_pmv.h"
#include "drol_pmva.h"
#include "drol_tun.h"
#include "drol_ech.h"
#include "drol_tdp.h"
#include "drol_prv.h"
#include "drol_imu.h"

#include "xzss.h"
#include "xzae103.h"
#include "xzac190.h"
#include "xzao00.h"

#define MAX_PRES_SITE 50

/* definition des variables globales a l'application */
XDY_District	vg_site;
XDY_Booleen	vg_presence_site[MAX_PRES_SITE];
XDY_Mot	vg_numero_plan;
XDY_Mot	vg_ordre_appel;
XDY_Mot	vg_ordre_tun;
XDY_Mot	vg_ordre_pmv;
XDY_Mot	vg_ordre_pmva;
XDY_Mot	vg_ordre_nav;
XDY_Mot	vg_ordre_fax;
XDY_Mot	vg_ordre_ech;
XDY_Mot vg_ordre_pal;
XDY_Mot vg_ordre_baf;
XDY_Mot vg_ordre_bad;
XDY_Mot vg_ordre_prv;
XDY_Mot vg_ordre_pic;
XDY_Booleen	vg_mode_historique;
XDY_Booleen	vg_calcul_auto;
XDY_Booleen	vg_calcul_pa_ci;
char		vg_poste[20];
XDY_NomSite vg_nom_site;	
int		vg_num_poste; 
int		vg_type_machine;
XZSCT_NomMachine NomMachine;
T_NOEUD *vg_B1[50];
T_NOEUD *vg_B2[50];
T_NOEUD *vg_B3[50];
T_NOEUD *vg_B4[50];
XDY_Mot		vg_SitePrincipal = 0;
XDY_NomSite vg_NomSitePrincipal;

/* definition des variables locales au module */
static char *	version = "drol_dir.c 1.57";


/* declaration de fonctions internes */
void drol_sortir();
void drol_dir_cb(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
static void drol_Presence_PCS();
void drol_process_Timer_KEEPALIVE_cb();
XDY_Booleen drol_lit_conf_calcul_pa();

/* definition de fonctions externes */


int main(int argc, char *argv[])
{
int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr	= XDC_OK;
T_IPC_MSG	pl_Msg;
XDY_NomSite	pl_NomSite;
int		vl_ValRet;
XDY_Datagroup	vl_dg;
XDY_Evt         NumEvt;
XDY_Horodate vl_horodate;
int vl_jour;
int j=0;

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
		 ,drol_sortir /* fontion de sortie */
		 ,"/dev/null"     /* redirection sortie standard */
		 ) ;


	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur XZSC_01InitCnxSup cr = %d",vl_cr);
	 	exit(1);
      	}
      	
	XZSC_07NomMachine(vg_poste);

      	/*A
        ** Recuperation du site de la machine
        */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, 
                	"%s : Recuperation du Nom du site impossible.",version);
                exit(XDC_NOK);
        }
        XZST_03EcritureTrace( XZSTC_WARNING, 
                	"%s : Recuperation du Nom du site %s.",version,pl_NomSite);

	if (!strcmp(pl_NomSite, XDC_NOM_SITE_CI))
		vg_site = XDC_CI;
	else if (!strcmp(pl_NomSite, XDC_NOM_SITE_VC))
		vg_site = XDC_VC;
	else if (!strcmp(pl_NomSite, XDC_NOM_SITE_DP))
		vg_site = XDC_DP;
	else if (!strcmp(pl_NomSite, XDC_NOM_SITE_CA))
		vg_site = XDC_CA;

	strcpy(vg_nom_site,pl_NomSite);

	vg_calcul_auto = FALSE;
	vg_calcul_pa_ci = drol_lit_conf_calcul_pa();

	vl_ValRet = XZSC_05TypeMachine(&vg_type_machine);
	switch (vg_type_machine) {
	case XZSCC_TYPEM_SD:	
	case XZSCC_TYPEM_SC:
			vg_mode_historique=FALSE;
			break;
	case XZSCC_TYPEM_PC2:
	case XZSCC_TYPEM_PC2LT:
		vg_mode_historique=FALSE;
		XZSC_07NomMachine(NomMachine);
		vl_cr = XZSS_07TransfertFichier(   NomMachine, "migrazur.kb.PC2", XDC_PATH_BDC_COURANTE,
					NomMachine, "migrazur.kb", XDC_PATH_BDC_COURANTE);
		if (vl_cr != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
					"Echec dans le transfert de migrazur.kb.PC2 en migrazur.kb");
		}
		break;

	default:
		XZSC_07NomMachine(NomMachine);
		if (!strcmp(NomMachine,"PODFA"))
			vg_mode_historique=TRUE;
		else
			vg_mode_historique=FALSE;
		break;
	}

	/*A 
	** Connexion a la base 
	*/
	if (vg_mode_historique)  strcpy(pl_NomSite,XDC_NOM_SITE_HIS);

        XZST_03EcritureTrace( XZSTC_WARNING,"SITE %s",pl_NomSite);
	if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
        {
                /*B
                ** Ecriture Trace
                */
                XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion au serveur SQL de %s impossible .",version,pl_NomSite);
                exit(1);
        }
        XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion au serveur SQL de %s reussie.",version,pl_NomSite);

	/*if (vg_type_machine==XZSCC_TYPEM_SD)
	{*/
	  XZST_03EcritureTrace( XZSTC_WARNING,"XDG_LX_IE");
	  strcpy(vl_dg, "XDG_LX_IE_");
	  strcat(vl_dg, pl_NomSite);
	  XDG_JoindreDG(vl_dg) ;
        /*}
	else {*/
	if (vg_mode_historique)
	  strcpy(vl_dg,"XDG_IE_DEV");
	else {
	  strcpy(vl_dg, "XDG_IE_");
	  strcat(vl_dg, pl_NomSite);
	}
	  strcpy(vl_dg, "XDG_IE_");
	  strcat(vl_dg, pl_NomSite);
        /*}*/
	if (XDG_JoindreDG(vl_dg) != T_TRUE)
		XZST_03EcritureTrace(XZSTC_FATAL,
			"probleme lors de l'abonnement au datagroup %s", vl_dg);
	XZST_03EcritureTrace(XZSTC_INFO, "abonnement au datagroup %s", vl_dg);

	//XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_MESSAGE|XZSTC_INTERFACE|XZSTC_FONCTION,XZSTC_NECRAN);
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_DEBUG1|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

	/* Abonnement au data group keep alive */
  	XDG_EncodeDG2(vl_dg, XDG_KEEPALIVE, pl_NomSite);
  	XDG_JoindreDG(vl_dg);

  	/* N'utilise pas le service generique car besoin appel proc stockee specifique */
  	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_KEEPALIVE),
      		drol_process_Timer_KEEPALIVE_cb,(T_CB_ARG) NULL)==NULL)
  	{
    		vl_ValRet = XZSCC_RTW_INV;
    		return (vl_ValRet);
  	}

	/* Lecture site principal en base */
	if (XZAO00_SitePrincipal(&vg_SitePrincipal,vg_NomSitePrincipal) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Site principal non defini en base RES_DIS.",version );
		return XDC_NOK;
	}


	/*chargement des donneees de config*/
	if (drol_tfm_init() != XDC_OK) 
	{ 
	  XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init tfm");
	  drol_sortir();
        }

	if (drol_fax_init() != XDC_OK)
	{
	    XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init fax");
	    drol_sortir();
        }

	if (drol_appel_init() != XDC_OK)
	{
	    XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init appel");
	    drol_sortir();
	}

	if (drol_pmv_init() != XDC_OK)
	{
	    XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init pmv");
	    drol_sortir();
	}

	if (drol_pmva_init() != XDC_OK)
	{
	    XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init pmva");
	    drol_sortir();
	}

	if (drol_nav_init() != XDC_OK)
	{
	  XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init BRA");
	  drol_sortir();
	}
	
	if (drol_tun_init() != XDC_OK)
	{
	  XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init TUN");
	  drol_sortir();
	}

	if (drol_ech_init() != XDC_OK)
	{
	  XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init ECH");
	  drol_sortir();
	}

	if (drol_pal_init() != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init PAL");
		drol_sortir();
	}

	if (drol_imu_init() != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init IMU");
		drol_sortir();
	}

	if (drol_baf_init() != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init BAF");
		drol_sortir();
	}
	if (drol_bad_init() != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init BAD");
		drol_sortir();
	}
	if (drol_tdp_init() != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init TDP");
		drol_sortir();
	}
	if (drol_prv_init() != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init PRV");
		drol_sortir();
	}
	if (drol_pic_init() != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_dir : erreur init PIC");
		drol_sortir();
	}
		XZST_03EcritureTrace (XZSTC_WARNING, "fin init");
	/*A
	**  armement des callbacks
	*/
      for ( j = 0; j < MAX_PRES_SITE-1 ; j ++ )
		vg_presence_site[j]=XDC_FAUX;

      /* appel a XZAE_103Abt_Presence_Op_PCsimpl*/ /* DEM1074 */
      if (vg_site == vg_SitePrincipal) {
      	if (XZAE103_Abt_Presence_Op_PCsimpl(XZEAC_DEBUT_ABONNEMENT,drol_Presence_PCS) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,": pb dans l'appel a XZAE103_Abt_Presence_Op_PCsimpl");
	
        }
      }

	
	if (TipcSrvProcessCbCreate(     XDM_IdentMsg(XDM_DIN_PA),
					drol_dir_cb,
					(T_CB_ARG)NULL) == NULL)
		XZST_03EcritureTrace(XZSTC_FATAL,
			"probleme lors de la creation du callback de reception de XDM_DIN_PA");

		XZST_03EcritureTrace (XZSTC_WARNING, "fin arme callback");

	/* Attente bloquante des messages du RTServer */
	while(1)
	{
		errno=0;
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
//				if (errno)
//					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
//						TutErrNumGet(), errno);
				}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tdrol %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				drol_sortir();
			}

			/*B INdication Non Presence RTserver */
//			if (errno)
//				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}

void drol_sortir()
{

	XZST_03EcritureTrace(XZSTC_FONCTION, "drol_sortir  : IN");

	XZAG03_Deconnexion_Base();

	/*A
	** Fermeture du fichier trace 
	*/
	
	XZST_02FermetureTrace();
	
	exit(0);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*       le callback de reception de XDM_DIN_PA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void     drol_dir_cb (va_Cnx, pa_Data, pa_Arg)
T_IPC_CONN                      va_Cnx;
T_IPC_CONN_PROCESS_CB_DATA      pa_Data;
T_CB_ARG                        pa_Arg;

/*
* ARGUMENTS EN ENTREE :
*   XDY_Horodate        Horodate
*   XDY_Eqt             PosteOperateur
*   XDY_Entier          NumEvt
*   XDY_Octet           NumEvtCle
*   XDY_Octet           TypeEvt
*   XDY_Booleen         TypePA
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun.
*
* CONDITION D'UTILISATION
*   callback de reception des messages XDM_DIN_PA
*
* FONCTION
*       - appel des services de description de l'evt
*       - demande d'un numero de plan d'actions
*	- calcul du PA
------------------------------------------------------*/
{
  XDY_Horodate    Horodate;
  XDY_Eqt         NumPosteOperateur;
#ifndef _TIBCO_EMS
  T_STR	           NomPosteOperateur;
#else
  char	           NomPosteOperateur[50];
#endif
  XDY_Entier      NumEvt;
  XDY_Octet       NumEvtCle;
  XDY_Octet       TypeEvt;
  XDY_Octet       TypePA;
  XDY_Mot	vl_mot;
  T_NOEUD	  *vl_noeud;
  XDY_Evt	  vl_Evt;
  int		  vl_indice;
  char		  vl_msg[10000]="";
  char		  vl_msg2[10000]="";
  XDY_Booleen	  vl_SMS_a_emettre_pc;
  XDY_Booleen	  vl_SMS_a_emettre_ci;
  XDY_Booleen	  vl_fax_a_emettre_pc;
  XDY_Booleen	  vl_fax_a_emettre_ci;
  XDY_Booleen	  vl_res;
  XDY_Booleen	  vl_res2;
  XDY_Booleen	vl_pas_beau;
  int	  vl_niveau=0;
  XDY_District	  vl_site;
  double	  vl_horodateDebut, vl_horodateFin;
  int		  vl_jourSemaine;
  int	vl_texte_restant = 0;
  int		  vl_duree;
  char vl_cible[6];
  int vl_premier_tfm = 0;
  XDY_Entier vl_action;
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : drol_dir_cb");
  XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_horodateDebut);
  char vl_texte[XDC_MAX_CHAR_LEN + 1];
  XDY_Act vl_action_tfm;
  /*A
  **  lecture par TipcMsgRead() du corps du message
  */
  if (TipcMsgRead(pa_Data->msg,
		  T_IPC_FT_REAL8, &Horodate,
		  T_IPC_FT_INT2, &NumPosteOperateur,
		  T_IPC_FT_STR, &NomPosteOperateur,
		  T_IPC_FT_INT4, &NumEvt,
		  T_IPC_FT_CHAR, &NumEvtCle,
		  T_IPC_FT_CHAR, &TypeEvt,
		  T_IPC_FT_CHAR, &TypePA,
		  NULL) != TRUE)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur dans TipcMsgRead");
  return ;
  }

  XZST_03EcritureTrace(XZSTC_MESSAGE,
		  "message recu : Horodate = %lf, NumPosteOperateur = %d, NomPosteOperateur = %s, NumEvt = % d, NumEvtCle = %d, TypeEvt = %d, TypePA = %d", Horodate, NumPosteOperateur, NomPosteOperateur, NumEvt, NumEvtCle, TypeEvt, TypePA);
  XZST_03EcritureTrace(XZSTC_MESSAGE,"vg_poste=%s",vg_poste);
    XZST_03EcritureTrace(XZSTC_MESSAGE,"vg_type_machine=%d",vg_type_machine);

  if (vg_type_machine!=XZSCC_TYPEM_SD) {
  	if (NumPosteOperateur==-1) {
      		vg_calcul_auto = XDC_VRAI;
	  	sprintf(vl_cible,"PO%s1",vg_nom_site);
	      	/*ne traiter la demande que sur POPR1, POCA1, POVE1, POVD1*/
		/*if (strcmp(vl_cible,NomMachine))
			return;*/
		}
	else
		vg_calcul_auto = XDC_FAUX;
  if (strcmp(NomPosteOperateur,vg_poste))
    return;
  }

  /*on memorise le numero du poste*/
  vg_num_poste = NumPosteOperateur;

  vl_Evt.numero=NumEvt;
  vl_Evt.cle=NumEvtCle;

  /*creation de l'arbre de la situation (causes + consequences)*/
  if (arbre_creer(NumEvt,NumEvtCle,TypeEvt,Horodate) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_dir_cb : pb appel arbre_creer");
    return;
  }

  /*recupere un numero de plan d'actions*/
  if (XZAP01_Creer_PA(Horodate, NumPosteOperateur, NumEvt, NumEvtCle, &vg_numero_plan) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_dir : erreur appel XZAP01_Creer_PA");
    return;
  }
/*DEBUT DEM427 */
  vg_ordre_pmv=10;
  vg_ordre_appel=40;
  vg_ordre_fax=60;
  vg_ordre_tun=1;
  vg_ordre_ech=35;
  vg_ordre_nav=30;
  vg_ordre_pal =20;
  vg_ordre_baf =25;
  vg_ordre_bad =27;
  vg_ordre_pmva =15;
  vg_ordre_prv =16;
  vg_ordre_pic =17;
/*FIN DEM427 */

  vl_noeud = vg_racine;

  strcpy(vl_msg,"\n");

  /*recuperation des infos sur les evts chaines*/
  while (vl_noeud!=NULL)
  {
    if (drol_tfm_lit_infos(Horodate,vl_noeud, vg_site)!=XDC_OK)
    {
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_dir_cb : pb appel drol_tfm_lit_infos");
	return;
    }

    vl_noeud=vl_noeud->Suivant;
  }
  if (arbre_trier() != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_dir_cb : pb appel arbre_trier");
    return;
  }
  
  vl_noeud=vg_racine;
  while (vl_noeud!=NULL){
    if ( (vl_noeud->NumEvt.numero == NumEvt) && (vl_noeud->NumEvt.cle==NumEvtCle)) 
    	break;
   vl_noeud=vl_noeud->Suivant;
   }
    /*diffusion sur les PO*/
    /* verif si deja msg RVZA diffuse*/
    if (XZAC126_Premier_RVA(NumEvt,NumEvtCle,&vl_premier_tfm) != XDC_OK) {
		 XZST_03EcritureTrace(XZSTC_WARNING,"pb appel XZAC126_Premier_Message");
		 vl_premier_tfm=0;
    }

    XZST_03EcritureTrace(XZSTC_WARNING , "premier %d",vl_premier_tfm);
 if (vl_premier_tfm==1) {
  if (  (vl_noeud->Type!=XZAEC_FMC_Contresens) &&
        (vl_noeud->Type!=XZAEC_FMC_VehMarcheArr) &&
	(vl_noeud->Type!=XZAEC_FMC_PietonChaussee) &&
	(vl_noeud->Type!=XZAEC_FMC_Accident)  &&
	(vl_noeud->Type!=XZAEC_FMC_VehFeu) &&
	(vl_noeud->Type!=XZAEC_FMC_Incendie) &&
	(vl_noeud->Type!=XZAEC_FMC_Obstacle) &&
	(vl_noeud->Type!=XZAEC_FMC_AnimalErrant) &&
	(vl_noeud->Type!=XZAEC_FMC_Regulation)
      ) {
    vl_premier_tfm=0;
  }

  if ( (vl_noeud->Type==XZAEC_FMC_VehPanne) ||
       (vl_noeud->Type==XZAEC_FMC_VehArrete)
     ) {
	vl_premier_tfm=0;
	if ( (vl_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) ||
         (vl_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE) ||
           (vl_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE) ||
            (vl_noeud->Localisation.VR==XDC_VOIE_BLOQUEE))
	    	vl_premier_tfm=1;
   }
 }

  if ((strstr(vl_noeud->Infos_TFM.Origine,"ESSAI") != NULL ) ||
      (strstr(vl_noeud->Infos_TFM.Origine,"EXERCICE") != NULL))
     	vl_premier_tfm=0;

  /* PAU SONO */
	XZST_03EcritureTrace(XZSTC_WARNING ,"drol sono numero =%d type =%d, vg_site=%d, Horodate:%lf",vl_noeud->NumEvt.numero, vl_noeud->Type,vg_site,Horodate);
  	/* Proposition appel PAU SONO si FMC incendie ou veh en feu Et que l'evt n'est pas termine*/
    if ( ((vl_noeud->Type == XZAEC_FMC_Incendie) || (vl_noeud->Type == XZAEC_FMC_VehFeu)) && !vl_noeud->Infos_TFM.Termine )							
	{
		drol_tun_filtre_sono(vl_noeud, vg_site, Horodate);
	}

  if (vl_noeud->Infos_TFM.Termine)  /* Si l evt est termine alors par de 1er msg RVA en auto & pas de proposition message RVA*/ 
  {
  	vl_premier_tfm=0;
	TypePA=XZDDC_PA_SANS_TFM ; /* pas de calcul message RVA */
  }

  /* IMU */
  if (!vl_noeud->Infos_TFM.Termine )
  	drol_imu_filtre(vl_noeud, vg_site, Horodate);


  vl_pas_beau=0;
  if (( ((TypePA==XZDDC_PA_AVEC_TFM)||(vl_premier_tfm==1)) &&
       (TypeEvt != XZAEC_FMC_AltercationUsager) &&
       (TypeEvt != XZAEC_FMC_MalaiseUsager) &&
       (TypeEvt != XZAEC_FMC_Agression) &&
       (TypeEvt != XZAEC_FMC_HoldUpVol) &&
       (TypeEvt != XZAEC_FMC_GreveESCOTA) 
     ) ||
     ((vg_calcul_pa_ci) && (!vl_noeud->Infos_TFM.Termine)))
  {
    XZST_03EcritureTrace(XZSTC_INFO,"drol_dir_cb : calcul avec message RVA");
 
    XZST_03EcritureTrace(XZSTC_DEBUG3,"%s",vl_msg);
    /*elaboration du message*/
    /*1e appel : 0 => recup Niveau (evt du PA)*/
    /*4 appels suivants => les 4 blocs B1 B2 B3 B3*/
    XZST_03EcritureTrace(XZSTC_DEBUG3,"bloc  %d",vl_niveau);
	strcat(vl_msg,drol_tfm_elabore(NumEvt,NumEvtCle,&vl_niveau,Horodate,0));
	XZST_03EcritureTrace(XZSTC_DEBUG3,"bloc %d %d",0,vl_niveau);
    strcat(vl_msg,drol_tfm_elabore(NumEvt,NumEvtCle,&vl_indice,Horodate,1));
    XZST_03EcritureTrace(XZSTC_DEBUG3,"bloc %d %d",1,vl_niveau);
	strcat(vl_msg,drol_tfm_elabore(NumEvt,NumEvtCle,&vl_indice,Horodate,2));
    XZST_03EcritureTrace(XZSTC_DEBUG3,"bloc %d %d",2,vl_niveau);
	strcat(vl_msg,drol_tfm_elabore(NumEvt,NumEvtCle,&vl_indice,Horodate,3));
    XZST_03EcritureTrace(XZSTC_DEBUG3,"bloc %d %d",3,vl_niveau);
	strcat(vl_msg,drol_tfm_elabore(NumEvt,NumEvtCle,&vl_indice,Horodate,4));
    XZST_03EcritureTrace(XZSTC_DEBUG3,"bloc %d %d",4,vl_niveau);
	XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_msg);

    vl_mot = (XDY_Mot)NumPosteOperateur;
    XZST_03EcritureTrace(XZSTC_WARNING , "premier %d // poste %d",vl_premier_tfm, vl_mot);
    if (vl_premier_tfm==1) {
	/*diffusion automatique a RVA*/
    	if (XZAC190_Envoyer_RVA(NumEvt,NumEvtCle,
				0,
				vg_site,
				vl_niveau,
				Horodate,
				-1,
				vl_mot , &vl_action) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"pb appel XZAC190_Envoyer_RVA");
    	 }
	 /*decoupage du message en bout de 250 caracteres*/
	 strcpy(vl_msg2, vl_msg);
	 do {
		strncpy(vl_texte, vl_msg2,XDC_MAX_CHAR_LEN);
		vl_texte[XDC_MAX_CHAR_LEN] =  '\0';
		if (XZAC200_Inserer_Texte(vl_action, vg_site, vl_texte) != XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"pb appel XZAC200_Inserer_Texte");
		}
		if (strlen(vl_msg2) >XDC_MAX_CHAR_LEN) {
			for (vl_indice = XDC_MAX_CHAR_LEN; vl_indice < strlen(vl_msg2); vl_indice ++)
		  		vl_msg[vl_indice-XDC_MAX_CHAR_LEN] = vl_msg2 [vl_indice];
			vl_msg[vl_indice-XDC_MAX_CHAR_LEN] =   '\0';
			strcpy(vl_msg2,vl_msg);		
			vl_texte_restant = 1;
	 	}
	 	else
			vl_texte_restant = 0;
	 }
	 while (vl_texte_restant ==1);

    	/*prevenir l adnimateur RVAa*/

	vl_action_tfm.numero = vl_action;
	vl_action_tfm.cle = vg_SitePrincipal;
	vl_pas_beau=1;
/*	XZIT06_Modif_Animateur_TFM(XDC_AJOUT, vl_action_tfm);*/

    }
    else {
      XZIT02_Message_TFM_Pret(  vl_Evt, NomPosteOperateur,
			XZITC_AVEC_MESSAGE_TFM, Horodate, 999, vl_niveau, vl_msg);
    }
  }

  vl_noeud=vg_racine;
  
  vl_fax_a_emettre_pc = FALSE;
  vl_fax_a_emettre_ci = FALSE;
  vl_SMS_a_emettre_pc = FALSE;
  vl_SMS_a_emettre_ci = FALSE;

  drol_appel_reinit();
  strcpy(vl_msg,"");
  while (vl_noeud!=NULL)
  {

    /*recuperation des interlocuteurs a prevenir pour chaque evt*/
    if ((vg_site==vg_SitePrincipal)  || vg_calcul_pa_ci)
    {
      vl_res = drol_appel_filtre(vl_noeud,vg_SitePrincipal,Horodate,&vl_res2);
      vl_fax_a_emettre_ci = vl_fax_a_emettre_ci || vl_res;
    	if ( (vl_noeud->NumEvt.numero == NumEvt) && (vl_noeud->NumEvt.cle==NumEvtCle)) 
      		vl_SMS_a_emettre_ci = vl_SMS_a_emettre_ci || vl_res2;

	vl_res = drol_appel_filtre(vl_noeud, vl_noeud->Localisation.Site,Horodate,&vl_res2);
       	vl_fax_a_emettre_pc = vl_fax_a_emettre_pc || vl_res;
    	if ( (vl_noeud->NumEvt.numero == NumEvt) && (vl_noeud->NumEvt.cle==NumEvtCle)) 
       		vl_SMS_a_emettre_pc = vl_SMS_a_emettre_pc || vl_res2;
    }
    else {
      /*JMG ne pas proposer dans un district des appels pour le district voisin*/
      /*DEM 13*/
      if (vl_noeud->Localisation.Site==vg_site) 
	{
        	vl_res = drol_appel_filtre(vl_noeud, vl_noeud->Localisation.Site,Horodate,&vl_res2);
        	vl_fax_a_emettre_pc = vl_fax_a_emettre_pc || vl_res;
    		if ( (vl_noeud->NumEvt.numero == NumEvt) && (vl_noeud->NumEvt.cle==NumEvtCle)) 
       			vl_SMS_a_emettre_pc = vl_SMS_a_emettre_pc || vl_res2;
        }
  }


/* PNI 21/06/07 : Traitement de baasculement pour travaux dans les 2 sens */
    if (vl_noeud->Type == XZAEC_FMC_BasculTravaux)
	vl_noeud->Localisation.Sens=vl_noeud->Localisation.Sens+10;

    if ( (vl_noeud->NumEvt.numero == NumEvt) && (vl_noeud->NumEvt.cle==NumEvtCle)) {
      vl_site=vl_noeud->Localisation.Site;

	/*si l'evt n'est pas fini*/
	if (!(vl_noeud->Infos_TFM.Termine)) 
	{
		/*recherche des perturbations pmv generees par l'evt*/
		drol_pmv_filtre(vl_noeud, vg_site, Horodate);

		drol_pmva_filtre(vl_noeud, vg_site, Horodate);

		if (!vg_calcul_auto) 
		{
			drol_nav_filtre(vl_noeud, vg_site, Horodate);

			drol_prv_filtre(vl_noeud, vg_site, Horodate);

			drol_pic_filtre(vl_noeud, vg_site, Horodate);

			if (vg_site != vg_SitePrincipal)  
			{
				drol_tun_filtre(vl_noeud, vg_site, Horodate,XDC_ACT_GTC_SIG);
				drol_tun_filtre(vl_noeud, vg_site, Horodate,XDC_ACT_GTC_VEN);
				drol_tun_filtre(vl_noeud, vg_site, Horodate,XDC_ACT_GTC_ECL);
			}
			else  
			{
				drol_tun_filtre(vl_noeud, XDC_DP, Horodate,XDC_ACT_GTC_SIG);
				drol_tun_filtre(vl_noeud, XDC_DP, Horodate,XDC_ACT_GTC_VEN);
				drol_tun_filtre(vl_noeud, XDC_DP, Horodate,XDC_ACT_GTC_ECL);
				drol_tun_filtre(vl_noeud, XDC_VC, Horodate,XDC_ACT_GTC_SIG);
				drol_tun_filtre(vl_noeud, XDC_VC, Horodate,XDC_ACT_GTC_VEN);
				drol_tun_filtre(vl_noeud, XDC_VC, Horodate,XDC_ACT_GTC_ECL);
			}

			drol_ech_filtre(vl_noeud, vg_site, Horodate);

			drol_baf_filtre(vl_noeud, vg_site, Horodate);

			drol_bad_filtre(vl_noeud, vg_site, Horodate);
		}
	}
  
      		drol_pal_filtre(vl_noeud, vg_site, Horodate);
	if (!vg_calcul_auto) {
      		/*drol_pal_filtre(vl_noeud, vg_site, Horodate);*/
      		drol_nav_filtre_pal(vl_noeud, vg_site, Horodate);
	}


      
      /* sur manif en cours au CI*/
      if ( (vg_site==vg_SitePrincipal) && (vl_noeud->Type == XZAEC_FMC_Manifestation) &&
	  (!(vl_noeud->Infos_TFM.Termine)))
        drol_fax_elabore_police(vl_noeud);

    }
    vl_noeud=vl_noeud->Suivant;
  }

  /*generation des cas d'appels TEL*/
  if (((vg_calcul_pa_ci) || (vg_site==vg_SitePrincipal)) && (!vg_calcul_auto) )
    drol_appel_elabore(vg_SitePrincipal);

  if ((vl_site!=XDC_DDE83)  && (!vg_calcul_auto))
    drol_appel_elabore(vl_site);

  /*generation des cas d'appels FAX si besoin*/
  
  XZST_03EcritureTrace(XZSTC_WARNING ,"valeur de vg_site: %d", vg_site);
  XZST_03EcritureTrace(XZSTC_WARNING ,"valeur de vl_site %d", vl_site);

  if ((vl_fax_a_emettre_ci)  || vg_calcul_pa_ci)
  {
	XZST_03EcritureTrace(XZSTC_FONCTION ,"vl_fax_a_emettre_ci");
	 strcat(vl_msg2,drol_fax_elabore(vg_SitePrincipal, vl_Evt,0,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 strcat(vl_msg2,drol_fax_elabore(vg_SitePrincipal, vl_Evt,1,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 strcat(vl_msg2,drol_fax_elabore(vg_SitePrincipal, vl_Evt,2,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 strcat(vl_msg2,drol_fax_elabore(vg_SitePrincipal, vl_Evt,3,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 drol_fax_elabore(vg_SitePrincipal, vl_Evt,4,vl_msg);
  }
  
  strcpy(vl_msg,"");
  strcpy(vl_msg2,"");
  if (vl_fax_a_emettre_pc) 
  {
	/*
	 * Modification Hilmarcher DEM 260 
	 * Envoi fax depuis PC 
	 */
	 
	XZST_03EcritureTrace(XZSTC_FONCTION ,"vl_fax_a_emettre_pc");
	 strcat(vl_msg2,drol_fax_elabore(vl_site, vl_Evt,0,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 strcat(vl_msg2,drol_fax_elabore(vl_site, vl_Evt,1,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 strcat(vl_msg2,drol_fax_elabore(vl_site, vl_Evt,2,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 strcat(vl_msg2,drol_fax_elabore(vl_site, vl_Evt,3,vl_msg));
	 strcpy(vl_msg,vl_msg2);
	 drol_fax_elabore(vl_site, vl_Evt,4,vl_msg);
  }

  XZST_03EcritureTrace(XZSTC_WARNING ,"valeur de vl_SMS_a_emettre_pc %d",vl_SMS_a_emettre_pc);
  XZST_03EcritureTrace(XZSTC_WARNING ,"valeur de vl_SMS_a_emettre_ci %d",vl_SMS_a_emettre_ci);
  if (vl_SMS_a_emettre_pc) {
  vl_noeud=vg_racine;
  while (vl_noeud!=NULL){
    if ( (vl_noeud->NumEvt.numero == NumEvt) && (vl_noeud->NumEvt.cle==NumEvtCle)) 
    	break;
   vl_noeud=vl_noeud->Suivant;
   }
	drol_sms_elabore(vl_site, vl_Evt, vl_noeud);
  }
  if (vl_SMS_a_emettre_ci) {
  vl_noeud=vg_racine;
  while (vl_noeud!=NULL){
    if ( (vl_noeud->NumEvt.numero == NumEvt) && (vl_noeud->NumEvt.cle==NumEvtCle)) 
    	break;
   vl_noeud=vl_noeud->Suivant;
   }
	drol_sms_elabore(vg_SitePrincipal, vl_Evt, vl_noeud);
  }

  /*proposition d'extinctions*/
  XZST_03EcritureTrace(XZSTC_INFO, "XZAP26 NO_PLAN=%d HORODATE=%lf EVT=%d CLE=%d ",vg_numero_plan, Horodate, NumEvt, NumEvtCle);
  if (XZAP26_Extinction(vg_numero_plan, Horodate, NumEvt, NumEvtCle) != XDC_OK) {
	XZAP26_Extinction(vg_numero_plan, Horodate, NumEvt, NumEvtCle) ;
  }

  if (!vg_calcul_auto) {
  	XZAP22_Avertir_PA_Pret(vg_numero_plan);
  	XZIT03_PA_Pret(vl_Evt, NomPosteOperateur, Horodate, vg_numero_plan, vl_niveau);
  }

  XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME,  &vl_jourSemaine, &vl_horodateFin);
  vl_duree = (int) (vl_horodateFin - vl_horodateDebut);

  {
	/* Traces des resultats */	
	char		vl_msgResultat[80];
	char		vl_nomFTrace[60];
	FILE		*vl_fTrace;
	time_t		vl_heureFin = (time_t) vl_horodateFin;



   // SAE_191 chantier date            
   char *tz;
   tz = getenv("TZ");
   setenv("TZ", "", 1);
   tzset();
   struct tm	*pl_date = localtime(&vl_heureFin);
   if (tz)
      setenv("TZ", tz, 1);
   else
      unsetenv("TZ");
   tzset();

	




	sprintf(vl_msgResultat, "CALCUL DU PLAN D'ACTION %d/%d EN %d SECONDES", (int) NumEvt, (int) NumEvtCle, vl_duree);
	XZST_03EcritureTrace(XZSTC_WARNING, vl_msgResultat);

	sprintf(vl_nomFTrace, "/tmp/TracesPlanAction_%d_%02d.log", pl_date->tm_year + 1900, pl_date->tm_mon + 1);
	vl_fTrace = fopen(vl_nomFTrace, "a");
	fprintf(vl_fTrace, "%02d/%02d/%d %02d:%02d:%02d %s\n", pl_date->tm_mday, pl_date->tm_mon + 1, pl_date->tm_year % 100,
	           pl_date->tm_hour, pl_date->tm_min, pl_date->tm_sec, vl_msgResultat);
	fclose(vl_fTrace);
  }

  /*liberation de l'arbre des evt*/
  while (vg_racine!=NULL) {
    vl_noeud=vg_racine;
    vg_racine=vg_racine->Suivant;
    free(vl_noeud);
  }

  if (vl_pas_beau==1)
	XZIT06_Modif_Animateur_TFM(XDC_AJOUT, vl_action_tfm);
}


/*************************************************
* MODULE  : DROL                                *
*************************************************
* DESCRIPTION :
* fonction utilisateur passe a
* XZAE103_Abt_Presence_Op_PCsimpl
*************************************************
* SEQUENCE D'APPEL :
*/

static void drol_Presence_PCS(
XZAET_EPresence_Op_PCsimpl      va_presence
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  int vl_site=0;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN: drol_presence_PCS");

  /*trace*/
  /* memorise l'etat du site */
  	if (!strcmp( va_presence.NomSite, XDC_NOM_SITE_CI))
		vl_site = XDC_CI;
	else if (!strcmp( va_presence.NomSite, XDC_NOM_SITE_VC))
		vl_site = XDC_VC;
	else if (!strcmp( va_presence.NomSite, XDC_NOM_SITE_DP))
		vl_site = XDC_DP;
	else if (!strcmp( va_presence.NomSite, XDC_NOM_SITE_CA))
		vl_site = XDC_CA;

	vg_presence_site[vl_site]=va_presence.Presence;

  

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT drol_presence_PCS presence %d, site %s %d",
				    va_presence.Presence,
				    va_presence.NomSite,
				    vl_site);

}

/*************************************************
* MODULE  : DROL                                *
*************************************************
* DESCRIPTION :
* Lecture du fichier de configuration fichiers/bdc/calcul_pa_ci.cfg 
* pour savoir si on doit calculer le plan d'action comme le CI
*************************************************
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_lit_conf_calcul_pa()

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR : Si le fichier contient 1 retourne TRUE sinon (pas de fichier ou different de 1) retourne FALSE
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
	char vl_str[200];
	FILE *pl_fd=NULL;
	char        vl_Lig[255];
	XDY_Entier vl_Resultat_out = 0;
	int vl_calcul;
	int vl_retcode;

	/*decodage du fichier des parametres a faire*/
	sprintf(vl_str,"/produits/migrazur/appliSD/fichiers/bdc/calcul_pa_ci.cfg");
	pl_fd=fopen(vl_str,"r");
	if (pl_fd==NULL) {
	  XZST_03EcritureTrace( XZSTC_WARNING,"pb ouverture fichier parametrage %s", vl_str);
	  return(FALSE);
	}

	vl_calcul=0;
        while ( ( vl_retcode = XZAO000_LireLigneFich ( pl_fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
	{
		if (vl_Resultat_out != XZAO_EOF ) 
		{
			if (vl_Lig[0]!='#') 
			{
				sscanf(vl_Lig,"%d", &vl_calcul);
				break;
			}
		}
	}
	fclose(pl_fd);

	return(vl_calcul==1);


}

void drol_process_Timer_KEEPALIVE_cb(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message timer KEEPALIVE");
  XZAP01_Creer_PA(0.0, 0, 0, 0, &vg_numero_plan);
}

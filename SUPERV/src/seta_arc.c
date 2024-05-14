/*E*/
/******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ARC * FICHIER seta_arc.c
******************************************************
* DESCRIPTION DU MODULE :
*	Module permettant de mettre en forme des messages a arguments
*	variables pour les diffuser, les afficher ou les enregistrer 
*	dans le journal d'administration
*seta_archiver_cb	Callback declenche sur la reception du msg T_MT_INFO afin
*	d'ecrire le message recu dans le journal d'administration
*seta_diffuser_msg : 	Permet de diffuser le message a la machine desiree
*seta_mesg_cb : 	Callback declenche sur la reception du msg XDM_TSE_MESG
*	recuperant le mesage pour l'afficher sur le synoptique
* seta_ecrire_msg : 	Ecrit le message passe en argument dans le fichier
*	d'administration
*seta_message : 	Recupere une liste variable d'arguments et cree
*	 un tableau de chaine de caracteres 
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 24 Oct 1994 	: Creation
* T.Milleville 01 Mar 1995 	: Modification de la fonction d'archivage 
*			de msg. On recupere fans le msg T_MT_INFO un seul
*			argument de type STR contenant tous les arguments
* T.Milleville 07 Mar 1995 	: Modification de XZGA_xx en XZSI_xx V1.7
* T.Milleville 08 Mar 1995 	: Modification de la modification XZSI_06
*	dans laquelle on specifie le destinataire V1.9
* T.Milleville 19 Avr 95 	: Ajout test pour diriger le msg 
*	sur le poste operateur ou administrateur V1.10
* T.Milleville 09 Juin  95 	: Ajout test pour verifioer si le msg
*	a archiver a comme numero de texte 0 V1.11
* T.Milleville 19 Jul  95 	: Modification sscanf en atoi V1.12
* T.Milleville 25 Jul  95 	: Ajout de commentaires V1.13
* T.Milleville 6 Aou  95 	: Modification traces V1.14
* T.Milleville 9 Aou  95 	: Modification traces V1.15
* T.Milleville 29 Sep  95 	: Lecture de la taille du Journal d'administartion 
*	la premiere fois. Donc variable globale V1.16
* T.Milleville 23 Oct  95 	: Modification code  dans seta_ecrire_msg
*		V1.17
* T.Milleville 07 Nov 95 	: Modification code  dans seta_ecrire_msg V1.19
* NIEPCERON	31 Jan 1996	: le destinataire est la machine aussi pour les msg adm (seta_mesg_cb) v1.20 
* NIEPCERON     02 Fev 1996	: Si le numero de message est nul alors on force le format de message a %s v1.21
* NIEPCERON	06 Nov 1997	: suppression du free sur var locale -> core en UX 10.20	v1.23
* JMG		26/06/10	: linux DEM/934 1.24
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : adequation format printf et type d'entier  1.25
* JMG		07/03/20 : EMS 1.26
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/
/* fichiers inclus */

#include <rtworks/ipc.h>

#include "xdc.h"
#include "xdm.h"
#include "xdg.h"
#include "xdf.h"
#include "xdl.h"

#include "xzsc.h"
#include "xzsa.h"
#include "xzst.h"
#include "xzse.h"
#include "xzsx.h"
#include "xzss.h"
#include "xzsi.h"
#include "xzia.h"

#include "seta_don.h"
#include "seta_arc.h"

/* definitions de constantes */
#define SETA_MAX_NBARG 10

/* definitions de types locaux */

typedef struct {
		  int NumTexte;
		  int NbArg;
		  void **TabArg;
               }
	       TM_INFOMSGARCH;

/* definition de macro locales */

/* declaration de variables locales */


static char *version = "$Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $ : seta_arc" ;

#define 	vg_FormatChar	"%s"

/* Variable globale pour taille du journal d'admyinistration */
/*	Modification V1.16 */
long         vg_TailleMax		= 0; 

/* declaration de fonctions internes */
void seta_ecrire_msg(void **pa_DonneesMsg);

/* definition de fonctions externes */



/*X*/
/* Fichier : $Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $	Release : $Revision: 1.26 $        Date :$Date: 2020/11/03 17:53:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ARC
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Declenche sur la reception du msg T_MT_INFO
*	Permet d'archiver dans le journal d'admnistration 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_archiver_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
*	Arguments d'une callback
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucun
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
    static char *version = "$Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $ : seta_archiver_cb" ;
    T_INT4              vl_NumTexte	= 0;        /* Id du message */
    T_INT4              vl_ArgNumerique	= 0;    /* pour recuperer 1 argument numerique */
    void                **pl_DonneesMsg;    /* caracteristiques d'un message d'archivage */
    XDLT_SystemeAlarme  vl_MessageCaract;
    T_INT4              vl_NbChamps;        /* nombre de champs du message T_MT_INFO */
    int                 vl_NumArg	= 0;          /* numero d'argument  */
    int                 vl_NbElem	= 0;          /* necessaire pour XZSA_19 */
    T_IPC_MT            vl_TypeMsg	= 0;
    T_STR		vl_ArgTrf	= "";

    XZSCT_NomMachine    vl_NomMachine	= "";
    int                 vl_TypeMachine	= 0;
    XZSCT_NomSite       vl_SiteMachine	= "";
    XZSCT_NomSite       pl_SiteLocal	= "";       /* site machine locale */
#ifndef _TIBCO_EMS
    T_STR               vl_TexteInfo	= "";
#else
    XDY_Commentaire vl_TexteInfo    = "";
#endif
    char         	vl_SepDiese[2]        = "#";
    char *       	pl_TextArg	= NULL;
    char *       	pl_TexteSauve	= NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_archiver_cb : IN \n");


	/*A Recuperation de la chaine de caracteres du Message T_MT_INFO */
	TipcMsgRead(pa_Data -> msg,
		    T_IPC_FT_STR, &vl_TexteInfo , NULL);

    XZST_03EcritureTrace(XZSTC_MESSAGE,
	"Recup Msg T_MT_INFO recup chaine %s \n",vl_TexteInfo);

    /*B recuperation du nom de la machine */
    pl_TexteSauve = (char *) malloc ( (strlen (vl_TexteInfo)+1) * sizeof(char));
    strcpy(pl_TexteSauve,vl_TexteInfo);

    pl_TextArg = strtok(pl_TexteSauve,vl_SepDiese);
    strcpy(vl_NomMachine,pl_TextArg);

    XZST_03EcritureTrace(XZSTC_DEBUG3,
	"Archivage T_MT_INFO nom machine %s \n",vl_NomMachine);

    /* creation de la liste des caracteristiques du message d'alarme */
    pl_DonneesMsg = malloc( ((size_t) vl_NumArg+1) * sizeof(void *));

    /*B recuperation des chaines du texte */
    pl_TextArg = strtok(NULL,vl_SepDiese);
    while ( pl_TextArg != NULL)
    {
    	XZST_03EcritureTrace(XZSTC_DEBUG3, "Archivage T_MT_INFO Recup Arg %s -- Num Arg %d -- long %d \n",	pl_TextArg,vl_NumArg,strlen(pl_TextArg) );
		pl_DonneesMsg[vl_NumArg++] = pl_TextArg;
    	XZST_03EcritureTrace(XZSTC_DEBUG3,"Valeur de pl_Donnees[%d] =  %s  \n", (vl_NumArg-1),pl_DonneesMsg[(vl_NumArg-1)]);
		pl_TextArg = strtok(NULL,vl_SepDiese);
    	pl_DonneesMsg = realloc( pl_DonneesMsg, ((size_t) vl_NumArg+1) * sizeof(void *));
    	XZST_03EcritureTrace(XZSTC_DEBUG3, "Archivage T_MT_INFO %d Argument  %s \n",vl_NumArg,pl_TextArg);
    }

    /* ajour du marqueur de fin */
    pl_DonneesMsg[vl_NumArg] = NULL;
	/*A Necessaire pour ems : on ne peut pas envoyer des tableaux avec des adresses non allouees, cela fait planter le TipcSrvMsgWrite */
	if (vl_NumArg<9)
	{
		for (;vl_NumArg<10;vl_NumArg++)
		{
			pl_DonneesMsg = realloc( pl_DonneesMsg, ((size_t) vl_NumArg+1) * sizeof(void *));
			pl_DonneesMsg[vl_NumArg] = NULL;
		}
	}

    /*A recuperer les caracteristiques du message */
    vl_NumTexte = atoi( pl_DonneesMsg[0]);
   /* sscanf(pl_DonneesMsg[0],"%d",&vl_NumTexte);*/
    if (vl_NumTexte != 0)
    {
	/*B Recherche des caracteristiques du texte */
    	XDL_01SystemeAlarme(vl_NumTexte,&vl_MessageCaract);
    }

    /*B TEST DU FLAG DE BASCULEMENT */
    if (vl_MessageCaract.IndBasc == XDLC_BASC)
    {
       /* recuperation du type de la machine */
       XZSA_17TypeMachine(vl_NomMachine,&vl_TypeMachine);

       if (vl_TypeMachine == XZSAC_TYPEM_SD)
       {
	  XZSE_07BasculementSD();
       }
       else if (vl_TypeMachine == XZSAC_TYPEM_SC)
       {
	  XZSA_23SiteMachine(vl_NomMachine,vl_SiteMachine);
          XZSE_05BasculementSC(vl_SiteMachine);
       }
    }


	XZST_03EcritureTrace(XZSTC_DEBUG1,"Flag de diffusion %d \n",
		vl_MessageCaract.IndDiff);


    /*B TEST DU FLAG DE DIFFUSION */
    switch (vl_MessageCaract.IndDiff) 
    {
       case XDLC_LOCAL :
	  seta_diffuser_msg(vl_NomMachine, 0, pl_DonneesMsg);
	  break;

       case XDLC_SITEEXP:
	  XZSA_23SiteMachine(vl_NomMachine,vl_SiteMachine);
	  XZSA_19ListeMachineSite(vl_SiteMachine, 
			seta_diffuser_msg, &vl_NbElem, pl_DonneesMsg);
          break;

       case XDLC_GLOBAL:
	  XZSA_01ListeMachine(&seta_diffuser_msg, &vl_NbElem, pl_DonneesMsg);
	  break;

       case XDLC_SITEADM :
	  XZSA_19ListeMachineSite(SEP_NOM_SITE, 
			&seta_diffuser_msg, &vl_NbElem, pl_DonneesMsg);
	  break;

       case XDLC_SITE :
	  XZSA_23SiteMachine(vl_NomMachine,vl_SiteMachine);
	  XZSA_19ListeMachineSite(vl_SiteMachine, 
			&seta_diffuser_msg, &vl_NbElem, pl_DonneesMsg);

	  /*A Teste si le Site de la machine est identique au
	  *	Site local */
	  if (!strcmp(SEP_NOM_SITE,vl_SiteMachine))
	  {
          	XZSA_19ListeMachineSite(SEP_NOM_SITE, 
			&seta_diffuser_msg, &vl_NbElem, pl_DonneesMsg);
	  }
	  break;

       default:
	  break; 
    }

    /*B ecriture dans journal d administration */
    seta_ecrire_msg(pl_DonneesMsg);

    /*B liberation memoire de la liste des caracteristiques 
    *			du message d'alarme */
    if (pl_TextArg != NULL)
    {
   	free(pl_TextArg);
   }
    if (pl_TexteSauve != NULL)
    {
   	free(pl_TexteSauve);
   }
   if (pl_DonneesMsg != NULL)
   {
   	free(pl_DonneesMsg);
   }
    
	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_archiver_cb : OUT \n");

}	/* Fin seta_archive_cb */


/*X*/
/* Fichier : $Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $	Release : $Revision: 1.26 $        Date :$Date: 2020/11/03 17:53:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ARC
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Permet de diffuser le message a la mchine desiree
*	Construction et envoie du msg XDM_TSE_MESG	
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_diffuser_msg(
	XZSCT_NomMachine pa_NomMachine,
        int              va_NumOrdre,
	void             **pa_DonneesMsg)

/*
* PARAMETRES EN ENTREE :
*	pa_NomMachine : Nom de la machine
*	va_NumOrdre   : Numero d'ordre
*       pa_DonneesMsg  : message a  diffuser
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $ : seta_diffuser_msg" ;
   int             vl_NumChaine	= 0;
   XZSCT_Datagroup pl_DataGroup	= "";
   int	vl_ValRet 		= XDC_OK;
   int	vl_NumTexte		= 0;
   int	i				= 0;
   int  vl_NULL			= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_diffuser_msg : IN \
	-- Nom mach %s -- Num Ordre %d \n",pa_NomMachine,va_NumOrdre);

   /* ajout de la destination */ 
   XDG_EncodeDG2(pl_DataGroup,XDG_NSUPETAT,pa_NomMachine);
      vl_NumTexte = atoi(pa_DonneesMsg[0]);

   /* on met des NULL du premier NULL rencontre a la fin du tableau sinon le TipcSrvWriteMsg plante car valeur non nulle mais non signifiante */
   for (i=1;i<10;i++)
   {
	   if (pa_DonneesMsg[i]==NULL)
	   	vl_NULL=1;

		if (vl_NULL==1)
			pa_DonneesMsg[i]=NULL;
   }


   if (pa_DonneesMsg != NULL) 
     {
   if (!TipcSrvMsgWrite(pl_DataGroup,
   		XDM_IdentMsg(XDM_TSE_MESG),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_INT4, vl_NumTexte,
		T_IPC_FT_STR, pa_DonneesMsg[0],
		T_IPC_FT_STR, pa_DonneesMsg[1],
		T_IPC_FT_STR, pa_DonneesMsg[2],
		T_IPC_FT_STR, pa_DonneesMsg[3],
		T_IPC_FT_STR, pa_DonneesMsg[4],
		T_IPC_FT_STR, pa_DonneesMsg[5],
		T_IPC_FT_STR, pa_DonneesMsg[6],
		T_IPC_FT_STR, pa_DonneesMsg[7],
		T_IPC_FT_STR, pa_DonneesMsg[8],
		T_IPC_FT_STR, pa_DonneesMsg[9],
		NULL))
     {
                     XZST_03EcritureTrace (XZSTC_WARNING,
		                     "seta_diffuser_msg: erreur d'ecriture du msg XDM_TSE_MSG");

				                     vl_ValRet = XDC_NOK;
           }
           }


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_diffuser_msg : OUT \
	-- Code Retour %d \n",vl_ValRet);

	return (vl_ValRet);

}	/* Fin seta_diffuser_msg */


/*X*/
/* Fichier : $Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $	Release : $Revision: 1.26 $        Date :$Date: 2020/11/03 17:53:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ARC
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Declenche sur la reception du msg DM_TSE_MESG
*	Traite le msg recu et l'affiche sur synoptique	
*
******************************************************
* SEQUENCE D'APPEL :
*/
void seta_mesg_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)
  
/*
* PARAMETRES EN ENTREE :
*	Argument d'une callback IPC
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $ : seta_mesg_cb" ;
   T_INT4 vl_NbChamps	= 0;    /* Nombre de champs du msg RT-WORKS */
   T_INT4 vl_NumChaine	= 0;   /* Numero de l argument dans le msg */
   T_INT4 vl_NumTexte	= 0;
#ifndef _TIBCO_EMS
   T_STR  pl_ArgMsg[SETA_MAX_NBARG];   /* arguments du msg */
#else
   XDY_Texte  pl_ArgMsg[SETA_MAX_NBARG];   /* arguments du msg */
#endif
   XZIAT_destinataire	vl_Destinataire;  


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_mesg_cb : IN \n");

#ifndef _TIBCO_EMS
   /* recuperation du nombre de champ du message */
   TipcMsgGetNumFields(pa_Data -> msg, &vl_NbChamps);

   /* recuperation du numero du texte */
   TipcMsgNextInt4(pa_Data -> msg, &vl_NumTexte);

   /* recuperation des arguments du message */
   for (vl_NumChaine = 0; 
	vl_NumChaine < (vl_NbChamps) && (vl_NumChaine < SETA_MAX_NBARG); 
	vl_NumChaine++)
   {
      TipcMsgNextStr(pa_Data -> msg, &pl_ArgMsg[vl_NumChaine]);
   }
   
   /* mise a NULL des arguments restants */
   for (; vl_NumChaine < SETA_MAX_NBARG; vl_NumChaine++)
   {
      pl_ArgMsg[vl_NumChaine] = NULL;
   }

#else

   TipcMsgRead(pa_Data -> msg,
   		T_IPC_FT_INT4,&vl_NumTexte,
		T_IPC_FT_STR, &pl_ArgMsg[0],
		T_IPC_FT_STR, &pl_ArgMsg[1],
		T_IPC_FT_STR, &pl_ArgMsg[2],
		T_IPC_FT_STR, &pl_ArgMsg[3],
		T_IPC_FT_STR, &pl_ArgMsg[4],
		T_IPC_FT_STR, &pl_ArgMsg[5],
		T_IPC_FT_STR, &pl_ArgMsg[6],
		T_IPC_FT_STR, &pl_ArgMsg[7],
		T_IPC_FT_STR, &pl_ArgMsg[8],
		T_IPC_FT_STR, &pl_ArgMsg[9],
		NULL);

#endif
   /*A Recuperation du Nom du Site */
   XZSC_04NomSite(vl_Destinataire.site);


   /*A affichage message systeme */

	/*A Test suivant le msg si on l'envoie dans la fentre operateur
	*	ou administarteur */
	if( (vl_NumTexte == XDL_SUP32_COMSIHS) ||
		(vl_NumTexte ==	XDL_SUP34_COMEXHS)  ||
		(vl_NumTexte ==	XDL_SUP39_SCHS)  ||
		(vl_NumTexte == XDL_SUP50_COMSCOK) ||
		(vl_NumTexte == XDL_SUP51_COMSDOK) || 
		(vl_NumTexte == XDL_SUP48_COMEXOK) )
	{
   		/*A Recuperation du Nom du Site */
   		XZSC_07NomMachine(vl_Destinataire.machine);

		/*B Affichage Operateur */
   		XZSI_06AffMsgSyst(vl_Destinataire,vl_NumTexte,
			pl_ArgMsg[0],pl_ArgMsg[1],pl_ArgMsg[2]
			,pl_ArgMsg[3],pl_ArgMsg[4],pl_ArgMsg[5]
			,pl_ArgMsg[6],pl_ArgMsg[7],pl_ArgMsg[8]
			,pl_ArgMsg[9],NULL);
	}
	else
	{ /* Ajout v1.20 */
		/*A Recuperation du Nom du Site */
		XZSC_07NomMachine(vl_Destinataire.machine);

		/*B Affichage Administrateur */
   		XZSI_08AffMsgAdm(vl_Destinataire,vl_NumTexte,
			pl_ArgMsg[0],pl_ArgMsg[1],pl_ArgMsg[2]
			,pl_ArgMsg[3],pl_ArgMsg[4],pl_ArgMsg[5]
			,pl_ArgMsg[6],pl_ArgMsg[7],pl_ArgMsg[8]
			,pl_ArgMsg[9],NULL);
	}


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_mesg_cb : OUT  \n");

}	/* Fin seta_mesg_cb */
 

/* definitions de fonctions internes */

/*X*/
/* Fichier : $Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $	$Revision: 1.26 $	Release : $Revision: 1.26 $        Date :$Date: 2020/11/03 17:53:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ARC
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction archive un message dans le fichier
* circulaire journald'administarion
* le fichier comporte un entete avec une structure
* du type XZSXT_EnteteJourAdm comprenant la
* taille max du fichier et le nombre d octets ecrits
* depuis le debut du fichier.
* si le fichier n existe pas, trace WARNING
* Elle prend en compte les eventuelles modifications
* du param Taille max ds fichier de conf XDF_NSUP_ADMINIS
*
******************************************************
* SEQUENCE D'APPEL :
*/
void seta_ecrire_msg(void **pa_DonneesMsg)
  
/*
* PARAMETRES EN ENTREE :
*	pa_DonneesMsg	: Message a ecrire dans le journal 
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $ : seta_ecrire_msg" ;
   FILE *fd_Journal		= NULL;
	/* Nom Complet du fichier journal */
   char  pl_NomFile[80]		= "";          
	/* Nom Complet du fichier de conf */
   char  pl_FicConf[80]		= "";          
   int   vl_TailleMsg         /* Taille utile du message y compris horodate */
	,vl_JourSemaine
	,vl_NumChaine;
   double vl_HoroSec		= 0.0;
   XZSXT_EnteteJourAdm vl_EnteteJournal	;
   char pl_ChaineEntete[50]		= "";

	/* Taille max du fichier journal ds fichier de conf */
#if 0	/* Modification V1.16 */
   long         vl_TailleMax		= 0; 
#endif

	/* texte enregistrement dans le fic journal */
   char  pl_LibMsg[255]			= "";      
   char  pl_LibMsgSav[255]			= "";      
   char  pl_LibMsgTmp[255]			= "";      
   char  pl_LibMsgTmpSav[255]			= "";      
   char  *pl_ChaineFormat			= NULL;      
   char  pl_ChaineFormatSav[255]			= "";      
   XZSMT_Horodate vl_Horodate		= "";
   XDLT_SystemeAlarme   vl_SysAlarme;
   int		vl_ValRet		= XDC_OK;
   int		vl_Length	= 0;
   int		vl_LengthSav	= 0;
   int		vl_NumTexte	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_ecrire_msg : IN  \n");


   /* lecture fichier de config et recuperation taille max
   *	si la taille n'est pas encore definie */
   if (vg_TailleMax == 0)
   { 
   	if(XZSX_29JournalLecture(&vg_TailleMax,XDC_NSUP_PATHOPER) != XDC_OK)
   	{
      		XZST_03EcritureTrace(XZSTC_WARNING,
			"seta_ecrire_msg : abort de XZSX_29JournalLecture");  
      		vg_TailleMax = 0;
   	}
    }

   /* recuperation nom du fichier journal complet */
   sprintf(pl_NomFile,"%s/%s",XDC_NSUP_PATHOPER,XDF_NSUP_JOURADM);

	/*A Verification de l'existance du Fichier Journal */
	if (access(pl_NomFile, F_OK) < 0)
	{
		/*B Fichier inexistant, donc creation */
		if (XZSS_16OuvrirFichierStream(XZSSC_FIC_WR_STREAM,
					pl_NomFile,&fd_Journal) != XDC_OK)
		{
		 	XZST_03EcritureTrace(XZSTC_WARNING,"seta_ecrire_msg : \
				 Erreur creation %s \n" ,pl_NomFile);
		}
	        else
	  	{
			/*B  ECriture de l'entete du Fichier */
			if (!fprintf(fd_Journal,XZSXC_FORMATENTETEJOURNAL,
				   vg_TailleMax, (XDY_LongEntier) XZSXC_TAILLEENTETEJOURNAL))
			{
				/*B Erreur ecriture entete Journal */
				XZST_03EcritureTrace(XZSTC_WARNING,
					"Erreur ecriture entete Journal \n");
			}
			else
			{
				/*B Fermeture Fichier Journal */
				XZSS_17FermerFichierStream( fd_Journal);
			}
		}
	}


   if (XZSS_16OuvrirFichierStream(XZSSC_FIC_RPLUS_STREAM,
			pl_NomFile,&fd_Journal) != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"seta_ecrire_msg : \
			Erreur ouverture %s \n" ,pl_NomFile);
   }
   else
   {
      /* lecture entete journal et recuperation Nombre octets ecrits */
      fseek(fd_Journal,0,SEEK_SET);
      fscanf(fd_Journal,XZSXC_FORMATENTETEJOURNAL,
	&vl_EnteteJournal.TailleMax,&vl_EnteteJournal.NbOctetEcrit);

      /* Si changement taillemax ds XDF_NSUP_ADMINIS
	 mais different de 0      ->MAJ vl_EnteteJournal */
      if (vl_EnteteJournal.TailleMax != vg_TailleMax && vg_TailleMax != 0)
      {
	XZST_03EcritureTrace( XZSTC_INFO, 
			"Changement Taille Max %d -- %d \n",
				vl_EnteteJournal.TailleMax,vg_TailleMax);

	/*B Fermeture Fichier Journal */
	XZSS_17FermerFichierStream( fd_Journal);

	/*B Destruction du journal d'administration */
	if (XZSS_06DetruireFichier(pl_NomFile))
	{ 
		XZST_03EcritureTrace( XZSTC_WARNING, 
			"pb a l'appel de XZSS_06DetruireFichier");
	}

	/*B Creation du Journal d'administration */
	if (XZSS_16OuvrirFichierStream(XZSSC_FIC_WR_STREAM,
				pl_NomFile,&fd_Journal) != XDC_OK)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"seta_ecrire_msg : \
			 Erreur creation %s \n" ,pl_NomFile);
	}
	else
 	{	
		/*B  ECriture de l'entete du Fichier */
		if (!fprintf(fd_Journal,XZSXC_FORMATENTETEJOURNAL,
			   vg_TailleMax, (XDY_LongEntier) XZSXC_TAILLEENTETEJOURNAL))
		{
			/*B Erreur ecriture entete Journal */
			XZST_03EcritureTrace(XZSTC_WARNING,
				"Erreur ecriture entete Journal \n");
		}
	}

	/*A Maj de la taille Max du fichier */
        vl_EnteteJournal.TailleMax = vg_TailleMax;
        vl_EnteteJournal.NbOctetEcrit = XZSXC_TAILLEENTETEJOURNAL; 
      }


      /* si fichier plein (vl_EnteteJournal.NbOctetEcrit >= vg_TailleMax ) 
	 MAJ du Nombre d octets ecrits */
      if (vl_EnteteJournal.NbOctetEcrit >= vl_EnteteJournal.TailleMax)
      {
         vl_EnteteJournal.NbOctetEcrit = XZSXC_TAILLEENTETEJOURNAL; 
      }

      /* positionnement dans journal */
      fseek(fd_Journal,vl_EnteteJournal.NbOctetEcrit,SEEK_SET);

      /* recuperation horodate */
      XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HoroSec);
      XZSM_11ConversionHorodate(vl_HoroSec,&vl_Horodate);

      /* creation enregistrement */
      if (pa_DonneesMsg != NULL)
      {
    	sscanf(pa_DonneesMsg[0],"%d",&vl_NumTexte);
	XZST_03EcritureTrace(XZSTC_DEBUG2,
		"Recuperation du Numero de Texte %d \n",vl_NumTexte);
      	/*B recuperation du format d'ecriture a partir de va_numTexte   */
	if (vl_NumTexte !=0)
	{
		vl_ValRet = XDL_01SystemeAlarme( 
			vl_NumTexte, &vl_SysAlarme );
	}
	else
	{
		vl_ValRet =  XDC_OK;
		strcpy(vl_SysAlarme.FormatFranc,"%s");
	}

	if ( vl_ValRet == XDC_NOK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "seta_archiver_cb : \
			Pb a l'appel de XDL_01SystemeAlarme");
	}
	else
	{
		sprintf( pl_LibMsg,"%s - %s",vl_Horodate,vl_SysAlarme.FormatFranc );

#if 0
		strcpy( pl_LibMsg, vl_Horodate );
		strcat( pl_LibMsg, " - " );
		strcat( pl_LibMsg, vl_SysAlarme.FormatFranc );
#endif


	 /* mise en place des chaines de caracteres */
	 strcpy(pl_LibMsgSav,pl_LibMsg);
         for (vl_NumChaine = 1; 
			pa_DonneesMsg[vl_NumChaine] != NULL; vl_NumChaine++) 
         {
		pl_ChaineFormat = NULL;
		pl_ChaineFormat = 
				strstr(pl_LibMsgSav,vg_FormatChar);
		vl_Length = (int) (strlen(pl_LibMsgSav) - (strlen(pl_ChaineFormat)-2));
		
		sprintf(pl_ChaineFormatSav,"%s", (pl_ChaineFormat+2));
#if 0
		strcpy(pl_ChaineFormatSav, (pl_ChaineFormat+2));
#endif

	 	strncpy(pl_LibMsgTmp,pl_LibMsgSav, vl_Length);
/* Modif V1.17 */
		pl_LibMsgTmp[vl_Length] = '\0';

		sprintf(pl_LibMsgSav,pl_LibMsgTmp,
					pa_DonneesMsg[vl_NumChaine]);

#if 0
		strcpy(pl_LibMsgTmp,pl_LibMsgSav);
	    	strcat(pl_LibMsgTmp,pl_ChaineFormatSav);
	    	strcat(pl_LibMsgTmp,"");
#endif
		sprintf(pl_LibMsgTmp,"%s%s",pl_LibMsgSav,pl_ChaineFormatSav);

		sprintf(pl_LibMsgSav,"%s",pl_LibMsgTmp);
		strcpy(pl_LibMsgTmp,"");
#if 0
		strcpy( pl_LibMsgSav,pl_LibMsgTmp);
		strcpy(pl_LibMsgTmp,"");
#endif

         }
       }
      }

	/*free (pl_ChaineFormat);*/ /*v1.23*/

      if (!fprintf(fd_Journal,"\t%s\n",pl_LibMsgSav))
      {
      	XZST_03EcritureTrace(XZSTC_WARNING,
		      "seta_ecrire_msg : erreur ecriture msg : %s" ,pl_LibMsg);
      }
      else
      {
	   /* flush de l ecriture */
	   fflush(fd_Journal);
      }

      /* Maj de l entete du journal */
      vl_EnteteJournal.NbOctetEcrit = ftell(fd_Journal);
      fseek(fd_Journal,0,SEEK_SET);


      if (!fprintf(fd_Journal,XZSXC_FORMATENTETEJOURNAL ,
		vl_EnteteJournal.TailleMax,vl_EnteteJournal.NbOctetEcrit))
      {
         XZST_03EcritureTrace(XZSTC_WARNING,
		"seta_ecrire_msg : erreur ecriture entete : %8ld %8ld"
		,vg_TailleMax,vl_EnteteJournal.NbOctetEcrit);

      }
      else fflush(fd_Journal);


      /* fermenture journal */
      fclose(fd_Journal);

   } /* ouverture fichier journal OK */

	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_ecrire_msg : OUT    \n");

}	/* Fin seta_ecrire_msg */






/*X*/
/* Fichier : $Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $	Release : $Revision: 1.26 $        Date :$Date: 2020/11/03 17:53:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ARC
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Recupere une liste variable d'arguments et cree
*	 un tableau de chaine de caracteres 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void **seta_message(int va_NumTexte, int va_NbChaine, ...)

/*
* PARAMETRES EN ENTREE :
*	va_NumTexte	: Numero du texte 
*	va_NbChaine	: 1er argument de la chaine variable 
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_arc.c,v 1.26 2020/11/03 17:53:30 pc2dpdy Exp $ : seta_message" ;
   static void **pl_DonneesMsg; /* adresse liste caracteristiques message */
   static int  vl_NumTexte	= 0;     /* numero de texte pour memorisation */
   va_list     pl_ListeArg;     /* adresse liste d'arguments variables */
   int         vl_NumChaine	= 0;    /* numero chaine en cours de traitement */
   char		vl_TextNum[5]	= "";

	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_message : IN    \n");

   /* allocation memoire pour recevoir les caracteristiques du message */
   pl_DonneesMsg = realloc(pl_DonneesMsg, ((size_t) va_NbChaine + 2) * sizeof(void *));

   /* positionnement sur l'adresse de la premiere chaine */
   va_start(pl_ListeArg, va_NbChaine);

   /* memorisation numero message */
   XZST_03EcritureTrace(XZSTC_WARNING,"seta_message : Num Texte %d \n",
		va_NumTexte);
   vl_NumTexte = va_NumTexte;
   sprintf(vl_TextNum,"%d",vl_NumTexte);
   pl_DonneesMsg[0]= vl_TextNum;
      XZST_03EcritureTrace(XZSTC_DEBUG2,"Donnees Msg [%d ] = %s --  %d \n",
		0,pl_DonneesMsg[0],vl_NumTexte);

   /* memorisation adresse de chaque chaine */
   for (vl_NumChaine = 1; vl_NumChaine <= va_NbChaine; vl_NumChaine++)
   {
      pl_DonneesMsg[vl_NumChaine] = va_arg(pl_ListeArg, void *);
      XZST_03EcritureTrace(XZSTC_DEBUG2,"Donnees Msg [%d ] = %s \n",
		vl_NumChaine,pl_DonneesMsg[vl_NumChaine]);
   }

   /* mise en place du marqueur de fin de caracteristiques */
   pl_DonneesMsg[va_NbChaine + 1] = NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_message : OUT     \n");

   return(pl_DonneesMsg); 

}	/* Fin seta_message */



/*E*/
/*Fichier : $Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $      Release : $Revision: 1.16 $        Date : $Date: 2018/10/04 10:22:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MLIV * FICHIER cgcd_liv.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de livraison de tcgcd gerant le configuration.
* (DCG-002/0.1).
**********************
* SERVICE RENDU : 
*	cliv_cb : Callback de la tache TCGCD effectuant la livraison.
**********************
* SERVICE RENDU : 
*	cliv_FctUtil_Gest_ligne_bl permet de traiter 
*  une ligne du Bon de livraison conformement a la fonctionalite
*  de livraison des fichiers par tcgcd.x (page34 du DCG-002/0.1).
****************
* SERVICE RENDU : 
*	cliv_Inform_Admin_Probl permet d informer l administrateur d un problème
* survenu lors de l execution d une demande de livraison ou de diffusion.
****************
* SERVICE RENDU : 
*  cliv_Modif_Tab_Checklist : Permet de changer l etat 
*  dans le tableau checklist de la verification
*  pour un type de fichier donne.
****************
* SERVICE RENDU : 
*  cliv_Consult_Tab_Checklist : Permet de recuperer l etat 
*  dans le tableau checklist de la verification
*  pour un type de fichier donne.
****************
* SERVICE RENDU : 
*  cliv_ConsultGlobale_Tab_Checklist : Renvoie XDC_OK si toutes les verif ont été effectuees
*  avec succes sinon XDC_NOK.
****************
* SERVICE RENDU : 
*  cliv_TypeFich_VerifGeree retourne XZCGC_ERR_VERIF si le type de fichier n est pas gere au niveau du tab de checklist
*	sinon on retourne le numero de verification.
*
****************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Dec 1994	: Creation
***************
* MERCIER	29 Dec 1994	: Ajout de cliv_ConsultGlobale_Tab_Checklist 	V 1.2
*	Mise à jour de l entete du fichier
***************
* MERCIER	28 Jan 1995	: Modif de la livraison dans l espace CONFIG	V 1.4
***************
* MERCIER	15 Mars 1995	: Ajout message admin pour parfaire
* 	la livraison dans l espace CONFIG					V 1.5
***************
* MERCIER	16 Mars 1995	: Ajout message admin pour parfaire
* 	l affichage de la livraison dans l espace CONFIG			V 1.6
***************
* MERCIER	16 Mars 1995	: Modification message a l administrateur	V 1.7
***************
* NIEPCERON     08 Jun  1995    : Ajout du traitement relatif aux fic. /inf
				  Correction du positionnement du flag de SYNP  V1.8$
***************
* NIEPCERON     22 Jun  1995    : Ajout d'un msg de fin de diffusion v1.9
***************
* NIEPCERON     08 Sep  1995    : Ajout de msg de de liv bien passee v1.9
*************
* NIECPERON     29 Jan 1996     : Ajout du ssysteme DIE v1.10
*************
* NIECPERON     05 Fev 1996     : Ajout du ssysteme MOD v1.11
*************
* NIECPERON     01 Mar 1996     : Correction des accents ds msg sys v1.12
* NIEPCERON     21 Oct 1996     : Ajout des ssysteme OPER UTIL et NTFM v1.13
* NIEPCERON     27 Jan 1999	: Ajout du ssyteme STRA 1.14 dem/1728
* JPL		13/07/16	: Concordance des nombre, type et valeur des arguments de messages  1.15
* JPL		20/02/18	: Migration Linux 64 bits (DEM 1274) : Controle des livraisons modifiant la base  1.16
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_util.h"

#include "cgcd_liv.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cgcd_liv" ;
	
static	int 	vg_FlagLiv_TYPEF_SCNT		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_SNAV		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_LPIC		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_MPMV		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EQAL		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_SYNP		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_RSAD		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_FAID           = XDC_NOK;
static  int     vg_FlagLiv_TYPEF_FINF		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_PIGN		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_DIE		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_MOD		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_MFBO		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_FADM		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_DORA		= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EQUI		= XDC_NOK;	
static	int 	vg_FlagLiv_TYPEF_DEFT		= XDC_NOK;	
static	int 	vg_FlagLiv_TYPEF_OPER		= XDC_NOK;	
static	int 	vg_FlagLiv_TYPEF_UTIL		= XDC_NOK;	
static	int 	vg_FlagLiv_TYPEF_NTFM		= XDC_NOK;	
static	int 	vg_FlagLiv_TYPEF_STRA		= XDC_NOK;	
static	int 	vg_FlagLiv_TYPEF_EXEC_BASEDD	= XDC_NOK;	
static	int 	vg_FlagLiv_TYPEF_EXEC_EQUEXT	= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EXEC_GECONF	= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EXEC_GESIHM	= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EXEC_MAINTE	= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EXEC_SAIDEC	= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EXEC_SIMTST	= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EXEC_SUPERV	= XDC_NOK;
static	int 	vg_FlagLiv_TYPEF_EXEC_XDMICG	= XDC_NOK;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*	cliv_cb : Callback de la tache TCGCD effectuant la livraison.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void cliv_cb( 		T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG)
/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback RTServer.
* ARGUMENTS EN SORTIE :
*   Aucun
* CODE RETOUR : 
*  	
* CONDITION D'UTILISATION
*   Aucune
* FONCTION 
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_cb";
	
	int 	vl_retcode	= XDC_OK;
	char 	vl_Buffer[100]	= "";
	char 	vl_mesgerr[80]	= "";
	char 	vl_Msg[500]	= "";
	int 	vl_Nb_ligne_BL	= 0;
	char 	vl_Jour[15]	= "";
	char 	vl_Heure[10]	= "";
	char 	*vl_JJ;
	char 	*vl_MM;
	char 	*vl_ANNEE;
	
	XZSMT_Horodate	pl_Horodate = "";	/* va contenir l horodate en caractere*/
	
	vg_FlagLiv_TYPEF_SCNT		= XDC_NOK;
	vg_FlagLiv_TYPEF_SNAV		= XDC_NOK;
	vg_FlagLiv_TYPEF_LPIC		= XDC_NOK;
	vg_FlagLiv_TYPEF_MPMV		= XDC_NOK;
	vg_FlagLiv_TYPEF_EQAL		= XDC_NOK;
	vg_FlagLiv_TYPEF_SYNP		= XDC_NOK;
	vg_FlagLiv_TYPEF_RSAD		= XDC_NOK;
	vg_FlagLiv_TYPEF_FAID           = XDC_NOK;
	vg_FlagLiv_TYPEF_FINF		= XDC_NOK;
	vg_FlagLiv_TYPEF_PIGN		= XDC_NOK;
	vg_FlagLiv_TYPEF_MFBO		= XDC_NOK;
	vg_FlagLiv_TYPEF_DIE		= XDC_NOK;
	vg_FlagLiv_TYPEF_MOD		= XDC_NOK;
	vg_FlagLiv_TYPEF_FADM		= XDC_NOK;
	vg_FlagLiv_TYPEF_DORA		= XDC_NOK;
	vg_FlagLiv_TYPEF_EQUI		= XDC_NOK;	
	vg_FlagLiv_TYPEF_DEFT		= XDC_NOK;	
	vg_FlagLiv_TYPEF_OPER		= XDC_NOK;
	vg_FlagLiv_TYPEF_UTIL		= XDC_NOK;	
	vg_FlagLiv_TYPEF_NTFM		= XDC_NOK;	
	vg_FlagLiv_TYPEF_STRA		= XDC_NOK;	
	vg_FlagLiv_TYPEF_EXEC_BASEDD	= XDC_NOK;	
	vg_FlagLiv_TYPEF_EXEC_EQUEXT	= XDC_NOK;
	vg_FlagLiv_TYPEF_EXEC_GECONF	= XDC_NOK;
	vg_FlagLiv_TYPEF_EXEC_GESIHM	= XDC_NOK;
	vg_FlagLiv_TYPEF_EXEC_MAINTE	= XDC_NOK;
	vg_FlagLiv_TYPEF_EXEC_SAIDEC	= XDC_NOK;
	vg_FlagLiv_TYPEF_EXEC_SIMTST	= XDC_NOK;
	vg_FlagLiv_TYPEF_EXEC_SUPERV	= XDC_NOK;
	vg_FlagLiv_TYPEF_EXEC_XDMICG	= XDC_NOK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cliv_cb est declenche");
	
	sprintf(vl_Msg," ");
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	sprintf(vl_Msg," ");
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);

	/*A
	** Lecture de l Horodate sous forme de caractere afin de constitue le nom
	** du fichier de bon de livraison avec comme suffixe l horodate courante.
	*/
	if ((vl_retcode =cuti_Recup_Horodate(pl_Horodate))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cuti_Recup_Horodate a echoue");

		XZST_03EcritureTrace(XZSTC_WARNING ,"OUT : cliv_cb sortie en erreur");
		return;
	}
	/*B
	** L horodate a la forme suivante : JJ/MM/ANNEE hh:mm:ss
	** On la decoupe en petite chaine pour obtenir un nom de fichier utilisable par le systeme.
	*/
	sscanf(pl_Horodate,"%s %s",vl_Jour,vl_Heure);
	vl_JJ		= strtok ( vl_Jour, "/" );
	vl_MM		= strtok ( NULL,    "/" );
	vl_ANNEE	= strtok ( NULL,    ":" );
	
	/*A
	** Copie du fichier XDF_BON_LIVRAISON du rep XDC_PATHREF de la machine 
	** de developpement dans le rep XDC_PATHCNF de la machine de developpement
	** avec l horodate courante en suffixe.
	*/
	sprintf(vl_Buffer,"%s.%s.%s.%s_%s",XDF_BON_LIVRAISON,vl_JJ,vl_MM,vl_ANNEE,vl_Heure);
	if((vl_retcode=XZSS_07TransfertFichier (XDC_MACHINE_GECONF,
						XDF_BON_LIVRAISON,
						XDC_PATHREFFICDYN,
						XDC_MACHINE_GECONF,
						vl_Buffer,
						XDC_PATHCNFFICDYN))!=XDC_OK)
	{
		/*B
		** Constitution d un message d erreur en fonction du retour
		*/
		switch (vl_retcode) 
		{
			case XZSSC_ARG_INV : strcpy(vl_mesgerr,"Argument incorrect");
			break;
			case XZSSC_FIC_INV : strcpy(vl_mesgerr,"Fichier inconnu");
			break;
			case XZSSC_ERR_SYS : strcpy(vl_mesgerr,"Erreur execution shell");
			break;
			default: sprintf(vl_mesgerr,"Le code d erreur n est pas connu : %d",vl_retcode);
			break;
		}
		/*B
		** En cas de prob d execution de la fonction utilisateur, on previent 
		** l administrateur.
		*/
		sprintf(vl_Msg,"Problème détecté pour copier le fichier %s vers %s : %s\n",XDF_BON_LIVRAISON,vl_Buffer,vl_mesgerr);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSS_07TransfertFichier a retourne le message : %s",vl_mesgerr);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_cb : Sortie en erreur.");
		return;
	}
	/*A
	** Tansfert du fichier XDF_BON_LIVRAISON du rep XDC_PATHREF de la machine 
	** de developpement dans le rep XDC_PATHCNF de la machine de developpement.
	*/
	if((vl_retcode=XZSS_07TransfertFichier (XDC_MACHINE_GECONF,
						XDF_BON_LIVRAISON,
						XDC_PATHREFFICDYN,
						XDC_MACHINE_GECONF,
						XDF_BON_LIVRAISON,
						XDC_PATHCNFFICDYN))!=XDC_OK)
	{
		/*B
		** Constitution d un message d erreur en fonction du retour
		*/
		switch (vl_retcode) 
		{
			case XZSSC_ARG_INV : strcpy(vl_mesgerr,"Argument incorrect");
			break;
			case XZSSC_FIC_INV : strcpy(vl_mesgerr,"Fichier inconnu");
			break;
			case XZSSC_ERR_SYS : strcpy(vl_mesgerr,"Erreur execution shell");
			break;
			default: sprintf(vl_mesgerr,"Le code d erreur n est pas connu : %d",vl_retcode);
			break;
		}
		/*B
		** En cas de prob d execution de la fonction utilisateur, on previent 
		** l administrateur.
		*/
		sprintf(vl_Msg,"Problème détecté sur le fichier %s : %s\n",XDF_BON_LIVRAISON,vl_mesgerr);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSS_07TransfertFichier a retourne le message : %s",vl_mesgerr);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_cb : Sortie en erreur.");
		return;
	}
	
	/*A
	** On declenche la lecture du BL avec la fonction utilisateur qui
	** pour chaque occurence du BL va effectuer la livraison et positionner
	** le tableau de checklist devant le type de fichier
	*/
	if((vl_retcode=xzcg02_Lecture_BL ( cliv_FctUtil_Gest_ligne_bl  , &vl_Nb_ligne_BL))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzcg02_Lecture_BL a retourne le code d erreur : %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INFO,"La livraison a echoue");
		/*B
		** En cas de prob d execution de la fonction utilisateur, on previent 
		** l administrateur.
		*/
		sprintf(vl_Msg,"Erreur de traitement sur le fichier BL %s\n",XDF_BON_LIVRAISON);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_cb : Sortie en erreur.");
		return;
	}


	sprintf(vl_Msg," ");
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	sprintf(vl_Msg,"Livraison Terminée.");
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	sprintf(vl_Msg," ");
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_cb : Sortie");
	return;	
}  
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*	cliv_FctUtil_Gest_ligne_bl permet de traiter 
*  une ligne du Bon de livraison conformement a la fonctionalite
*  de livraison des fichiers par tcgcd.x (page34 du DCG-002/0.1).
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_FctUtil_Gest_ligne_bl(XZCGT_ComposantBL va_LigneBL,int *pa_retcode_out )

/*
* ARGUMENTS EN ENTREE :
*   	va_LigneBL
* ARGUMENTS EN SORTIE :
*   	pa_retcode_out : va contenir le status du traitement de l enregistrement 
* 	du BL en cours de traitement.
* CODE RETOUR : 
*	XDC_OK
*	XDC_NOK
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*	Pour chaque occurence du fichier XDF_BON_LIVRAISON :
*	- Initialise en erreur de l element du tableau Checklist correspondant
*  au type de fichier.
*	- Construction du repertoire du fichier d origine selon le type fichier :
*	Si type de fichier = EXEC_sous-systeme : XDC_PATHREF + sous-systeme + XDC_PATHBIN.
*
*	Si type de fichier  = autre : XDC_PATHREFFIC+ nom du rep, le nom du repertoire
*	depend du type de fichier.
*	
*	- Contruction du repertoire du fichier destinataire selon le type de fichier :
*	Si type de fichier = EXEC_sous-systeme : XDC_PATHCNFEXE
*
*	Si type de fichier = autre : XDC_PATHCNFFICH + nom du rep, le nom du repertoire
*	depend du type de fichier.
*
*	-Si type de fichier est different de GLOBAL : Transfert du fichier 
*	avec la primitive XZSS07 avec les arguments :
*		Nom de la machine origine  : machine de developpement
*		Nom du repertoire origine defini comme ci-dessus
*		Nom du fichier origine : nom du fichier
*	
*		Nom de la machine destinataire : Serveur de donnees CI (XZSC07)
*		Nom du repertoire destinataire defini comme ci-dessus
*		Nom du fichier destinataire : nom du fichier
*
*	-Si type de fichier est GLOBAL : Transfert du repectoire avec le module MTRF avec
*	les arguments :
*		Nom de la machine origine  : machine de developpement
*   		Nom du repertoire origine defini comme ci-dessus
*		
*		Nom de la machine destinataire : Serveur de donnees CI (XZSC07)
*		Nom du repertoire destinataire defini comme ci-dessus
*
*
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_FctUtil_Gest_ligne_bl";
	int 	vl_retcode	= XDC_OK;
	char 	vl_mesgerr[250]	= "";
	
	int	vl_numerr	= 0;
	char 	vl_Msg[1000]	= "";
	char	vl_commande[500] = "";
	char	vl_line[250]	 = "";
	FILE	*vl_ShellD;
	int	vl_Status	= XDC_OK;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cliv_FctUtil_Gest_ligne_bl  : debut d execution");
	
	/*A
	**	Type de fichier a livrer vide ? 
	*/
	if (va_LigneBL.TypeFic == NULL)
	{
		sprintf(vl_Msg,"LIVRAISON : erreur type de fichier (1 ere colonne) vide");
		if((vl_retcode = XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : l IHM de l administrateur a retourné un problème \nsur l envoi du message suivant : %s\n",vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_cb : Fin d execution");
			return(XDC_NOK);
		}
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
		return(XDC_NOK);
	}
	/*A
	**  Comparaison de va_LigneBL.TypeFic avec les types connus.
	*/
	/*A
	** Livraison des fichiers des seuils de niveau de trafic.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SCNT))&&(vg_FlagLiv_TYPEF_SCNT == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_SCNT = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_SCNT_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_SCNT_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}		
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_SCNT_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);		
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SCNT))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_SCNT_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers des scenarios NAV.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SNAV))&&(vg_FlagLiv_TYPEF_SNAV == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_SNAV = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_SNAV_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_SNAV_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_SNAV_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);	
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SNAV))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_SNAV_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de listes des Pictogrammes.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_LPIC))&&(vg_FlagLiv_TYPEF_LPIC == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_LPIC = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_LPIC_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_LPIC_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_LPIC_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_LPIC))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_LPIC_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers des messages PMV.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MPMV))&&(vg_FlagLiv_TYPEF_MPMV == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_MPMV = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_MPMV_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_MPMV_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_MPMV_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MPMV))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_MPMV_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers des equations d alertes.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EQAL))&&(vg_FlagLiv_TYPEF_EQAL == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EQAL = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EQAL_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EQAL_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EQAL_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EQAL))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EQAL_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers des synoptiques.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SYNP))&&(vg_FlagLiv_TYPEF_SYNP == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_SYNP = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_SYNP_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_SYNP_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_SYNP_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SYNP))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_SYNP_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers des regles du sad.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_RSAD))&&(vg_FlagLiv_TYPEF_RSAD == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_RSAD = XDC_OK;
		/*A Modification de l etat Checklist pour ce type de fichier*/
		if((vl_retcode=cliv_Modif_Tab_Checklist(va_LigneBL.TypeFic,XDC_NOK))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist passage du typeFic %d a %d avant %s a echoue",
					va_LigneBL.TypeFic,
					XDC_NOK,
					CLIVC_LIVRAISON);
			sprintf(vl_Msg,"%s : Prise en compte de la demande de %s de %s de type %s a echoué",
					CLIVC_LIVRAISON,
					"changement d'etat",
					va_LigneBL.NomFichier,
					va_LigneBL.TypeFic);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");		
			return(XDC_NOK);	 
		}
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_RSAD_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_RSAD_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_RSAD_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_RSAD))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_RSAD_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers d inf.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FINF))&&(vg_FlagLiv_TYPEF_FINF == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_FINF = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(   CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_FINF_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_FINF_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
                        XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			 XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			 return(XDC_NOK);
                }
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_FINF_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
        }
	else
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FINF))
		{
			sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
                                                 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_FINF_CONFIG,
						 XDC_MACHINE_GECONF);
                        XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
                }
        }


	
	/*A
	** Livraison des fichiers d aides.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FAID))&&(vg_FlagLiv_TYPEF_FAID == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_FAID = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_FAID_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_FAID_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_FAID_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FAID))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_FAID_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de synoptiques IGN.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_PIGN))&&(vg_FlagLiv_TYPEF_PIGN == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_PIGN = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_PIGN_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_PIGN_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_PIGN_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_PIGN))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_PIGN_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de synoptiques DIE.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DIE))&&(vg_FlagLiv_TYPEF_DIE == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_DIE = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_DIE_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_DIE_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_DIE_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DIE))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_DIE_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de synoptiques MOD.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MOD))&&(vg_FlagLiv_TYPEF_MOD == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_MOD = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_MOD_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_MOD_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_MOD_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MOD))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_MOD_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de synoptiques NTFM.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_NTFM))&&(vg_FlagLiv_TYPEF_NTFM == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_NTFM = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_NTFM_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_NTFM_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_NTFM_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_NTFM))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_NTFM_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de STRA.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_STRA))&&(vg_FlagLiv_TYPEF_STRA == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_STRA = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_STRA_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_STRA_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_STRA_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_STRA))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_STRA_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de synoptiques OPER.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_OPER))&&(vg_FlagLiv_TYPEF_OPER == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_OPER = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_OPER_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_OPER_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_OPER_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_OPER))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_OPER_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de synoptiques UTIL.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_UTIL))&&(vg_FlagLiv_TYPEF_UTIL == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_UTIL = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_UTIL_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_UTIL_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_UTIL_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_UTIL))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_UTIL_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers de macros boites a outils.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MFBO))&&(vg_FlagLiv_TYPEF_MFBO == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_MFBO = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_MFBO_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_MFBO_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_MFBO_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MFBO))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_MFBO_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers d administration.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FADM))&&(vg_FlagLiv_TYPEF_FADM == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_FADM = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_FADM_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_FADM_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_FADM_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FADM))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_FADM_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des donnees du reseau autoroutier
	*/	
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DORA))&&(vg_FlagLiv_TYPEF_DORA == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_DORA = XDC_OK;
		/*A Modification de l etat Checklist pour ce type de fichier*/
		if((vl_retcode=cliv_Modif_Tab_Checklist(va_LigneBL.TypeFic,XDC_NOK))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist passage du typeFic %d a %d avant %s a echoue",
					va_LigneBL.TypeFic,
					XDC_NOK,
					CLIVC_LIVRAISON);
			sprintf(vl_Msg,"%s : Prise en compte de la demande de %s de %s de type %s a echoué",
					CLIVC_LIVRAISON,
					"changement d'etat",
					va_LigneBL.NomFichier,
					va_LigneBL.TypeFic);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");		
			return(XDC_NOK);	 
		}
		/*A Modification de l etat Checklist pour l etat de sauvegarde de la Base*/
		if((vl_retcode=cliv_Modif_Tab_Checklist(XZCGC_TYPEF_TRANSFERT_BASE,XDC_NOK))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist : Modification du Flag Transfert de Base a echoue");
					
			sprintf(vl_Msg,"%s : Problème interne : Modification du Flag Transfert de Base a echoué", version);
					
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");		
			return(XDC_NOK);	 
		}
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_DORA_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_DORA_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_DORA_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DORA))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_DORA_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des donnees des equipements.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EQUI))&&(vg_FlagLiv_TYPEF_EQUI == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EQUI = XDC_OK;
		/*A Modification de l etat Checklist pour ce type de fichier*/
		if((vl_retcode=cliv_Modif_Tab_Checklist(va_LigneBL.TypeFic,XDC_NOK))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist passage du typeFic %d a %d avant %s a echoue",
					va_LigneBL.TypeFic,
					XDC_NOK,
					CLIVC_LIVRAISON);
			sprintf(vl_Msg,"%s : Prise en compte de la demande de %s de %s de type %s a echoué",
					CLIVC_LIVRAISON,
					"changement d'etat",
					va_LigneBL.NomFichier,
					va_LigneBL.TypeFic);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");		
			return(XDC_NOK);	 
		}
		/*A Modification de l etat Checklist pour l etat de sauvegarde de la Base*/
		if((vl_retcode=cliv_Modif_Tab_Checklist(XZCGC_TYPEF_TRANSFERT_BASE,XDC_NOK))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist : Modification du Flag Transfert de Base a echoue");
					
			sprintf(vl_Msg,"%s : Problème interne : Modification du Flag Transfert de Base a echoué", version);
					
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");		
			return(XDC_NOK);	 
		}
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EQUI_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EQUI_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EQUI_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
			
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EQUI))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EQUI_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des fichiers par defaut
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DEFT))&&(vg_FlagLiv_TYPEF_DEFT == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_DEFT = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_DEFT_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_DEFT_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_DEFT_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}	
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DEFT))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_DEFT_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables du sous-systeme BASEDD
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_BASEDD))&&(vg_FlagLiv_TYPEF_EXEC_BASEDD == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_BASEDD = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_BASEDD_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_BASEDD_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_BASEDD_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_BASEDD))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_BASEDD_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
	}
	/*A
	** Livraison des executables du sous-systeme EQUEXT.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_EQUEXT))&&(vg_FlagLiv_TYPEF_EXEC_EQUEXT == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_EQUEXT = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_EQUEXT_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_EQUEXT_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_EQUEXT_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_EQUEXT))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_EQUEXT_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables du sous-systeme GECONF.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_GECONF))&&(vg_FlagLiv_TYPEF_EXEC_GECONF == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_GECONF = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_GECONF_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_GECONF_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_GECONF_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_GECONF))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_GECONF_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables du sous-systeme GESIHM.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_GESIHM))&&(vg_FlagLiv_TYPEF_EXEC_GESIHM == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_GESIHM = XDC_OK;
		
		
		sprintf(vl_commande,"%s/cgcd_ApDif.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CLIVC_LIVR_EXEC_GESIHM_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CLIVC_LIVR_EXEC_GESIHM_CONFIG);
		 			
		sprintf(vl_Msg,"Demande de %s des macros applix contenu dans %s sur %s de la machine %s",
					CLIVC_LIVRAISON,
					CLIVC_LIVR_EXEC_GESIHM_SOURCE,
					CLIVC_LIVR_EXEC_GESIHM_CONFIG,
					XDC_MACHINE_GECONF);
					
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
			
		vl_Status = XDC_OK;
		/* Recuperation des macros applix de reference dans l espace de config*/
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur système : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
					vl_Status = XDC_NOK;
				}
			}
			if (vl_Status == XDC_NOK)
			{
				sprintf(vl_Msg,"%s : Toutes les macros Applix du rep %s n ont pas été livrées sur %s : une erreur a été détectée",
					CLIVC_LIVRAISON,
					CLIVC_LIVR_EXEC_GESIHM_SOURCE,
					XDC_MACHINE_GECONF);
						
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
				sprintf(vl_Msg,"%s : Verifiez les droits des fichiers ou du repertoire %s de %s",
						CLIVC_LIVRAISON,
						CLIVC_LIVR_EXEC_GESIHM_CONFIG,
						XDC_MACHINE_GECONF);
			}
			else
			{
				sprintf(vl_Msg,"%s : Toutes les macros Applix du rep %s ont été livrées sur %s .",
						CLIVC_LIVRAISON,
						CLIVC_LIVR_EXEC_GESIHM_SOURCE,
						XDC_MACHINE_GECONF);
			}
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
				
		}
		else
		{
			sprintf(vl_Msg,"%s : les macros Applix du rep %s n'ont pas été livrées sur %s : l'éxécution de la commande %s a echoué",
					CLIVC_LIVRAISON,
					CLIVC_LIVR_EXEC_GESIHM_SOURCE,
					XDC_MACHINE_GECONF,
					vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier : Problème d execution de la commande %s",vl_commande);	
		}
		pclose(vl_ShellD);
			
		
		
		
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_GESIHM_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_GESIHM_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_GESIHM_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_GESIHM))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_GESIHM_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables du sous-systeme MAINTE.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_MAINTE))&&(vg_FlagLiv_TYPEF_EXEC_MAINTE == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_MAINTE = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_MAINTE_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_MAINTE_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_MAINTE_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_MAINTE))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_MAINTE_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables du sous-systeme SAIDEC.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SAIDEC))&&(vg_FlagLiv_TYPEF_EXEC_SAIDEC == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_SAIDEC = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_SAIDEC_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_SAIDEC_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_SAIDEC_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SAIDEC))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_SAIDEC_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables du sous-systeme SIMTST.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SIMTST))&&(vg_FlagLiv_TYPEF_EXEC_SIMTST == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_SIMTST = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_SIMTST_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_SIMTST_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_SIMTST_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SIMTST))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_SIMTST_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables du sous-systeme SUPERV.
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SUPERV))&&(vg_FlagLiv_TYPEF_EXEC_SUPERV == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_SUPERV = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_SUPERV_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_SUPERV_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_SUPERV_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SUPERV))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_SUPERV_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	/*A
	** Livraison des executables de XDMICG (au cas ou).
	*/
	if((!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_XDMICG))&&(vg_FlagLiv_TYPEF_EXEC_XDMICG == XDC_NOK))
	{
		vg_FlagLiv_TYPEF_EXEC_XDMICG = XDC_OK;
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CLIVC_LIVRAISON,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CLIVC_LIVR_EXEC_XDMICG_SOURCE,
					XDC_MACHINE_GECONF,
					CLIVC_LIVR_EXEC_XDMICG_CONFIG,
					XDC_MACHINE_GECONF))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a echoue ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s sur la machine %s",
				CLIVC_LIVRAISON,
				va_LigneBL.NomFichier,
				va_LigneBL.TypeFic,
				CLIVC_LIVR_EXEC_XDMICG_CONFIG,	
				XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
		return(XDC_OK);
	}
	else 
	{
		if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_XDMICG))
		{
		 	sprintf(vl_Msg,"%s : %s de type %s est à jour dans %s sur la machine %s",
						 CLIVC_LIVRAISON,
						 va_LigneBL.NomFichier,
						 va_LigneBL.TypeFic,
						 CLIVC_LIVR_EXEC_XDMICG_CONFIG,
						 XDC_MACHINE_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}
	}
	

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_FctUtil_Gest_ligne_bl : Sortie ");
	return(XDC_OK);
	
} 
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*
*	cliv_Inform_Admin_Probl permet d informer l administrateur d un problème
* survenu lors de l execution d une demande de livraison ou de diffusion.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cliv_Inform_Admin_Probl ( 	char * va_Type_Transfert,
				char * va_Fich_source ,
				char * va_Rep_source ,
				char * va_Mach_source, 
				char * va_Fich_Dest,
				char * va_Rep_Dest,
				char * va_Mach_Dest,
				int    va_NumErr_XZSS_07)
/*
* ARGUMENTS EN ENTREE :
*	va_NumErr_XZSS_07 : code d erreur de xzss_07 qui permet de cp ou rcp des fichiers.
*  d une machine vers une autre.
*
* ARGUMENTS EN SORTIE :
* CODE RETOUR : 
*	XDC_OK
*	
* CONDITION D'UTILISATION
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : ";
	int 	vl_retcode	= XDC_OK;
	char 	vl_mesg[500]	= "";
	char 	vl_mesgerr[50]	= "";
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : cliv_Inform_Admin_Probl : Debut d execution" );
	/*B
	** Constitution d un message d erreur en fonction du retour
	*/
	switch (va_NumErr_XZSS_07) 
	{
		case XZSSC_ARG_INV : strcpy(vl_mesgerr,"Argument incorrect");
		break;
		case XZSSC_FIC_INV : strcpy(vl_mesgerr,"Fichier inconnu");
		break;
		case XZSSC_ERR_SYS : strcpy(vl_mesgerr,"Erreur execution shell");
				break;
		default: sprintf(vl_mesgerr,"Le code d erreur n est pas connu : %d",vl_retcode);
				break;
	}
	sprintf(vl_mesg,"TCGCD : La %s de %s:%s/%s \nvers %s:%s/%s a generer le message d erreur suivant :\n\t%s\n",
	  		va_Type_Transfert,
	  		va_Mach_source,
	 	 	va_Rep_source,
	 	 	va_Fich_source,
	 	 	va_Mach_Dest,
	 	 	va_Rep_Dest,
	 	 	va_Fich_Dest,
		  	vl_mesgerr);
	
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");	  
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_mesg);
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_Inform_Admin_Probl : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cliv_Modif_Tab_Checklist : Permet de changer l etat 
*  dans le tableau checklist de la verification
*  pour un type de fichier donne.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_Modif_Tab_Checklist(char * va_TypeFic , int va_Flag)

/*
* ARGUMENTS EN ENTREE :
*   	va_TypeFic contient de type de fichier que l on doit positionne a va_flag
*	va_Flag	doit contenir XDC_OK ou XDC_NOK.	
* ARGUMENTS EN SORTIE :
*   	un tableau de checklist global est modifie a la valeur du flag sur le type de fichier 
*  defini.
* CODE RETOUR : 
*	XDC_OK
*	XDC_NOK
* CONDITION D'UTILISATION
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_Modif_Tab_Checklist";
	int 	vl_retcode	= XDC_OK;
	int 	vl_indice	= 0;
	int 	vl_Fin		= XDC_NOK;
	int	vl_Type_Verif	= 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cliv_Modif_Tab_Checklist a recu TypeFichier = %s a positionne a %d",
						va_TypeFic , 
						va_Flag);
	/*A
	** Test si le type de fichier est valide
	*/
	if ( cliv_Verif_TypeFich(va_TypeFic) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist\n Erreur : Type de fichier recu (%d) a passer a %s incorrect",
					va_Flag,va_TypeFic);
		return(XDC_NOK);
	}
	/*A
	** Test si la valeur du flag est valide
	*/
	if ((va_Flag != XDC_OK)&&(va_Flag != XDC_NOK))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist\n Erreur sur la valeur du flag %d pour le type de fichier %s",
					va_TypeFic,va_Flag);
	}
	
	/*A
	** Changement d etat du type de fichier a livre 
	*/
	vl_Fin 		= XDC_NOK;
	vl_indice 	= 0;
	/*A
	** On recupere le type de verif associe au type de fichier.
	*/
	vl_Type_Verif 	= cliv_TypeFich_VerifGeree(va_TypeFic);
	/*A
	** Test si lon a un type de verif associe au type de fichier.
	*/
	if(vl_Type_Verif == XZCGC_ERR_VERIF)
	{
		XZST_03EcritureTrace(XZSTC_INFO, "cliv_Modif_Tab_Checklist : Pas de verification associee au type de fichier %s",va_TypeFic);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_Modif_Tab_Checklist : Fin d execution");
		return(XDC_OK);
	}	
	while (( vl_indice < XZCGC_NB_VERIF) && ( vl_Fin != XDC_OK))
	{
		if(vl_Type_Verif ==  CLIVG_Tab_Checklist[vl_indice].TypeVerif)
		{
			CLIVG_Tab_Checklist[vl_indice].Flag = va_Flag;
			XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_Modif_Tab_Checklist modifie l etat de la verification de %d a %d",
				CLIVG_Tab_Checklist[vl_indice].TypeVerif ,
				CLIVG_Tab_Checklist[vl_indice].Flag);
			vl_Fin = XDC_OK;
		}
		vl_indice++;
	}/*while*/
	
	/*
	** Test de sortie de boucle
	*/
	if(vl_Fin != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Modif_Tab_Checklist : Problème dans la gestion du tableau de checklist");
	}
	/*A
	** Ecriture sur disque du nouvel etat du tableau Checklist
	*/
	/*B
	** On se positionne au debut du fichier
	*/
	fseek(CGCDG_Fich_CHECKLIST,0L,SEEK_SET);
	for(vl_indice =0 ; vl_indice < XZCGC_NB_VERIF ; vl_indice++)
	{
		if(((vl_retcode = fprintf(CGCDG_Fich_CHECKLIST,"%d %d\n",
			CLIVG_Tab_Checklist[vl_indice].TypeVerif,
			CLIVG_Tab_Checklist[vl_indice].Flag)) == EOF) || (vl_retcode <=0))
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"cliv_Modif_Tab_Checklist : Ecriture impossible dans le fichier de checklist de l enregistrement %d",
				vl_indice);
			return( XDC_NOK);	
		}
	}
	if (( vl_retcode = fflush(CGCDG_Fich_CHECKLIST))== EOF)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"cliv_Modif_Tab_Checklist : Impossible de flusher le fichier de checklist sur disque");
		return(XDC_NOK);
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_Modif_Tab_Checklist : Fin d execution");
	return(XDC_OK);
	
} 
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cliv_Consult_Tab_Checklist : Permet de recuperer l etat 
*  dans le tableau checklist de la verification
*  pour un type de fichier donne.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_Consult_Tab_Checklist(char * va_TypeFic, int * pa_Etat)

/*
* ARGUMENTS EN ENTREE :
*   	va_TypeFic contient de type de fichier dont on doit rendre l etat de verification.	
* ARGUMENTS EN SORTIE :
*   	un tableau de checklist global est modifie a la valeur du flag sur le type de fichier 
*  defini.
* CODE RETOUR : 
*	XDC_OK
*	XDC_NOK
* CONDITION D'UTILISATION
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_Consult_Tab_Checklist";
	int 	vl_retcode	= XDC_OK;
	int 	vl_indice	= 0;
	int 	vl_Fin		= XDC_NOK;
	int	vl_Type_Verif	= 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cliv_Consult_Tab_Checklist a recu TypeFichier = %s a verifier",va_TypeFic);
	
	*pa_Etat = XDC_OK;
	
	/*A
	** Test si la verification Checklist est en place ?
	** Si CGCDV_CHECKLIST == XDC_NOK alors on n effectue pas de verif.
	*/
	if(CGCDV_CHECKLIST == XDC_NOK)
	{
		*pa_Etat = XDC_OK;
		return(XDC_OK);
	}
	/*A
	** Lecture sur disque du nouvel etat du tableau Checklist
	*/
		/*B
		** On se positionne au debut du fichier
		*/
	fseek(CGCDG_Fich_CHECKLIST,0L,SEEK_SET);
		/*B
		** Lecture ligne a ligne du tableau de chechlist
		*/
	for(vl_indice = 0 ; vl_indice < XZCGC_NB_VERIF ; vl_indice++)
	{
		if(((vl_retcode = fscanf(CGCDG_Fich_CHECKLIST,"%d %d\n",
			&(CLIVG_Tab_Checklist[vl_indice].TypeVerif),
			&(CLIVG_Tab_Checklist[vl_indice].Flag))) == EOF) || (vl_retcode <=0))
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"cliv_Consult_Tab_Checklist : Lecture impossible du fichier de checklist de l enregistrement %d",
				vl_indice);
			return( XDC_NOK);	
		}
	}
	/*A
	** Recherche du type de fichier afin de renvoyer son etat de verification.
	*/
	
	/*A
	** Recuperation de l etat du type de fichier a diffuser 
	*/
	vl_Fin 		= XDC_NOK;
	vl_indice 	= 0;
	/*A
	** Recupere le type de verif associe au type de fichier
	*/
	
	vl_Type_Verif 	= cliv_TypeFich_VerifGeree(va_TypeFic);
	/*A
	** Test si lon a un type de verif associe au type de fichier.
	*/
	if(vl_Type_Verif == XZCGC_ERR_VERIF)
	{
		XZST_03EcritureTrace(XZSTC_INFO, "cliv_Consult_Tab_Checklist : Pas de verification associee au type de fichier %s",va_TypeFic);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_Consult_Tab_Checklist : Fin d execution");
		*pa_Etat=XDC_OK;
		return(XDC_OK);
	}
	while (( vl_indice < XZCGC_NB_VERIF) && ( vl_Fin != XDC_OK))
	{
		if(vl_Type_Verif  ==  CLIVG_Tab_Checklist[vl_indice].TypeVerif)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_Consult_Tab_Checklist recupere l etat %d de la verification de %d",
				CLIVG_Tab_Checklist[vl_indice].Flag ,
				CLIVG_Tab_Checklist[vl_indice].TypeVerif);
			vl_Fin = XDC_OK;
		}
		vl_indice++;
	}
	/*A
	** On retourne l etat de Verification
	*/
	if (vl_Fin == XDC_OK )
	{
		*pa_Etat	=	CLIVG_Tab_Checklist[vl_indice-1].Flag;
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_Consult_Tab_Checklist a trouve l etat %d pour le type de fichier %d",
					CLIVG_Tab_Checklist[vl_indice-1].Flag,
					CLIVG_Tab_Checklist[vl_indice].TypeVerif);
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_Consult_Tab_Checklist : Fin d execution");
	return(XDC_OK);	
} 
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cliv_ConsultGlobale_Tab_Checklist : Renvoie XDC_OK si toutes les verif ont été effectuees
*  avec succes sinon XDC_NOK.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_ConsultGlobale_Tab_Checklist()

/*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
* CODE RETOUR : 
*	XDC_OK
*	XDC_NOK
* CONDITION D'UTILISATION
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_ConsultGlobale_Tab_Checklist";
	int 	vl_retcode	= XDC_OK;
	int 	vl_indice	= 0;
	int 	vl_Fin		= XDC_NOK;
	int	vl_Type_Verif	= 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cliv_ConsultGlobale_Tab_Checklist :Debut d execution");
	
	/*A
	** Test si la verification Checklist est en place ?
	** Si CGCDV_CHECKLIST == XDC_NOK alors on n effectue pas de verif.
	*/
	if(CGCDV_CHECKLIST == XDC_NOK)
	{
		
		return(XDC_OK);
	}
	/*A
	** Lecture sur disque du nouvel etat du tableau Checklist
	*/
		/*B
		** On se positionne au debut du fichier
		*/
	fseek(CGCDG_Fich_CHECKLIST,0L,SEEK_SET);
		/*B
		** Lecture ligne a ligne du tableau de chechlist
		*/
	for(vl_indice = 0 ; vl_indice < XZCGC_NB_VERIF ; vl_indice++)
	{
		if(((vl_retcode = fscanf(CGCDG_Fich_CHECKLIST,"%d %d\n",
			&(CLIVG_Tab_Checklist[vl_indice].TypeVerif),
			&(CLIVG_Tab_Checklist[vl_indice].Flag))) == EOF) || (vl_retcode <=0))
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"cliv_ConsultGlobale_Tab_Checklist : Lecture impossible du fichier de checklist de l enregistrement %d",
				vl_indice);
			return( XDC_NOK);	
		}
	}
	/*A
	** Recherche dans le tableau de checklist, si une verification est toujours positionnee a Faux
	*/
	while (( vl_indice < XZCGC_NB_VERIF) && ( vl_Fin != XDC_OK))
	{
		if(CLIVG_Tab_Checklist[vl_indice].Flag == XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_ConsultGlobale_Tab_Checklist recupere l etat %d de la verification de %d",
				CLIVG_Tab_Checklist[vl_indice].Flag ,
				CLIVG_Tab_Checklist[vl_indice].TypeVerif);
			vl_Fin = XDC_OK;
		}
		vl_indice++;
	}
	/*A
	** Si on a trouve un etat de verif positionne a faux, on retourne une erreur.
	*/
	if (vl_Fin == XDC_OK )
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_ConsultGlobale_Tab_Checklist a trouve l etat %d pour le type de fichier %d",
					CLIVG_Tab_Checklist[vl_indice-1].Flag,
					CLIVG_Tab_Checklist[vl_indice].TypeVerif);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_ConsultGlobale_Tab_Checklist : Fin d execution : la Verification n a pas été effectue avec succes sur tous les types de fichiers");
		return(XDC_NOK);	
	}
	/*A
	** Si on n a pas trouve de flag mis a faux alors les verification ont été effectuee: On retourne OK
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_ConsultGlobale_Tab_Checklist : Fin d execution");
	return(XDC_OK);	
} 
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cliv_TypeFich_VerifGeree retourne XZCGC_ERR_VERIF si le type de fichier n est pas gere au niveau du tab de checklist
*	sinon on retourne le numero de verification.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_TypeFich_VerifGeree (char * pa_TypeFic )

/*
* ARGUMENTS EN ENTREE :
*   	va_TypeFic contient de type de fichier a verifier
*		
* ARGUMENTS EN SORTIE :
*   	
* CODE RETOUR : 
*	retourne XZCGC_ERR_VERIF si le type de fichier n est pas gere au niveau du tab de checklist
*	sinon on retourne le numero de verification.
* CONDITION D'UTILISATION   
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_TypeFich_VerifGeree";
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cliv_TypeFich_VerifGeree a recu TypeFichier = %s ",pa_TypeFic );
	
	
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_SCNT)) return (XZCGC_VERIF_SCNT);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_SNAV)) return (XZCGC_VERIF_SNAV);
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_LPIC)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_MPMV)) return ();*/
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EQAL)) return (XZCGC_VERIF_EQAL);
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_SYNP)) return ();*/
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_RSAD)) return (XZCGC_VERIF_RSAD);
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_FAID)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_FINF)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_PIGN)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_DIE)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_MOD)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_MFBO)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_FADM)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_UTIL)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_OPER)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_NTFM)) return ();*/
	/*if(!strcmp(pa_TypeFic,XZCGC_TYPEF_STRA)) return ();*/
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_DEFT)) return (XZCGC_VERIF_DEFT);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_DORA)) return (XZCGC_VERIF_RESE);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EQUI)) return (XZCGC_VERIF_RESE);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_TRANSFERT_BASE)) return (XZCGC_VERIF_TRANSFERTBASE);
	
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_GLOBAL)) return (XZCGC_VERIF_GLOBAL);
	
	else 
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_TypeFich_VerifGeree : SORTIE");
		return (XZCGC_ERR_VERIF);
	}
} 

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cliv_Verif_TypeFich retourne XDC_OK si le typeFichier passe en argument
*  est un type connu dans xzcg.h sinon retourne XDC_NOK.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_Verif_TypeFich(char * pa_TypeFic )

/*
* ARGUMENTS EN ENTREE :
*   	va_TypeFic contient de type de fichier a verifier
*		
* ARGUMENTS EN SORTIE :
*   	
* CODE RETOUR : 
*	XDC_OK si le type de fichier est correct 
*	sinon XDC_NOK
* CONDITION D'UTILISATION   
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_Verif_TypeFich";
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cliv_Verif_TypeFich a recu TypeFichier = %s ",pa_TypeFic );
	
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_SCNT)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_SNAV)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_LPIC)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_MPMV)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EQAL)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_SYNP)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_RSAD)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_FAID)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_FINF)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_PIGN)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_MFBO)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_DIE)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_MOD)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_FADM)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_DEFT)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_DORA)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EQUI)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_UTIL)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_OPER)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_NTFM)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_STRA)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_BASEDD)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_EQUEXT)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_GECONF)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_GESIHM)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_MAINTE)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_SAIDEC)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_SIMTST)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_SUPERV)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_EXEC_XDMICG)) return (XDC_OK);
	if(!strcmp(pa_TypeFic,XZCGC_TYPEF_TRANSFERT_BASE)) return (XDC_OK);
	
	if(strcmp(pa_TypeFic,XZCGC_TYPEF_GLOBAL) != 0)
		return (XDC_NOK);

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cliv_Verif_TypeFich : SORTIE");
	return (XDC_OK);
} 

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cliv_Init_Tab_Checklist : Initialise le tableau checklist
*  avec le fichier XDF_CHECKLIST qui se trouve dans XDC_PATHCNF.
*  Si le fichier n existe pas alors on le constitue avec des flags XDC_NOK
*  pour les types de fichiers connus.
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_Init_Tab_Checklist()

/*
* ARGUMENTS EN ENTREE :
*   	
*		
* ARGUMENTS EN SORTIE :
*   	
* CODE RETOUR : 
*	XDC_OK si le type de fichier est correct 
*	sinon XDC_NOK
* CONDITION D'UTILISATION   
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version 		= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_Init_Tab_Checklist";
	FILE *	pl_FD;
	int	vl_FisrtChecklist	= XDC_OK;
	int	vl_continue 		= XDC_OK;
	int 	vl_indice		= 0;
	int 	vl_retcode		= 0;
	char	pl_NomFile[250] 	= "";
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cliv_Init_Tab_Checklist" );
	
	
	/*A
	** Test d existance du fichier de checklist des versions logiciel
	*/
	sprintf(pl_NomFile,"%s/%s",XDC_PATHFICDYN,XDF_CHECKLIST);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** Test si le fichier existe deja
		*/
		vl_FisrtChecklist = XDC_OK;	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_Init_Tab_Checklist : ouverture du fichier de travail %s reussi",pl_NomFile);
		vl_FisrtChecklist = XDC_NOK;
	}
	fclose(pl_FD);
	/*A
	** Si ne fichier n existe pas, on le cree
	*/
	if(vl_FisrtChecklist == XDC_OK)
	{
		if (!(pl_FD=fopen(pl_NomFile,"w+")))
		{
			/*B 
			** Si on n arrive pas cree le fichier : sortie en fatal :
			** on est dans l incapacite de gerer la coherence des versions logiciel
			*/
			XZST_03EcritureTrace(XZSTC_FATAL,"cliv_Init_Tab_Checklist : Creation impossible du fichier de checklist %s",pl_NomFile);
			return( XDC_NOK);	
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_Init_Tab_Checklist : Creation du fichier de travail %s reussi",pl_NomFile);
			
			/*B
			** Initialisation du tableau de checklist a faux
			*/
			CLIVG_Tab_Checklist[0].TypeVerif 	= XZCGC_VERIF_SCNT;
			CLIVG_Tab_Checklist[0].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[1].TypeVerif 	= XZCGC_VERIF_SNAV;
			CLIVG_Tab_Checklist[1].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[2].TypeVerif 	= XZCGC_VERIF_DEFT;
			CLIVG_Tab_Checklist[2].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[3].TypeVerif 	= XZCGC_VERIF_SIHM;
			CLIVG_Tab_Checklist[3].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[4].TypeVerif 	= XZCGC_VERIF_RESE;
			CLIVG_Tab_Checklist[4].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[5].TypeVerif 	= XZCGC_VERIF_TYPE;
			CLIVG_Tab_Checklist[5].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[6].TypeVerif 	= XZCGC_VERIF_EQAL;
			CLIVG_Tab_Checklist[6].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[7].TypeVerif 	= XZCGC_VERIF_EQUI;
			CLIVG_Tab_Checklist[7].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[8].TypeVerif 	= XZCGC_VERIF_RSAD;
			CLIVG_Tab_Checklist[8].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[9].TypeVerif 	= XZCGC_VERIF_SGBD;
			CLIVG_Tab_Checklist[9].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[10].TypeVerif 	= XZCGC_VERIF_MACH;
			CLIVG_Tab_Checklist[10].Flag		= XDC_NOK;
			
			
			CLIVG_Tab_Checklist[11].TypeVerif 	= XZCGC_VERIF_GLOBAL;
			CLIVG_Tab_Checklist[11].Flag		= XDC_NOK;
			
			CLIVG_Tab_Checklist[12].TypeVerif 	= XZCGC_VERIF_TRANSFERTBASE;
			CLIVG_Tab_Checklist[12].Flag		= XDC_NOK;
			
			for(vl_indice =0 ; vl_indice < XZCGC_NB_VERIF ; vl_indice++)
			{
				if(((vl_retcode = fprintf(pl_FD,"%d %d\n",
					CLIVG_Tab_Checklist[vl_indice].TypeVerif,
					CLIVG_Tab_Checklist[vl_indice].Flag)) == EOF) || (vl_retcode <=0))
				{
					fflush(pl_FD);
					
					XZST_03EcritureTrace(XZSTC_FATAL,"cliv_Init_Tab_Checklist : Ecriture impossible dans le fichier de checklist %s de l enregistrement %d",
							pl_NomFile,
							vl_indice);
					
					return( XDC_NOK);	
				}
			}
			
			if (( vl_retcode = fflush(pl_FD))== EOF)
			{
				XZST_03EcritureTrace(XZSTC_FATAL,"cliv_Init_Tab_Checklist : Impossible de flusher le fichier de checklist %s sur le disque",
								pl_NomFile);
			}
		}
	}
	else
	{
		/*A
		** Le fichier existe deja : Charge l etat courant 
		** de la verification de la coherence des versions logiciel
		*/
		if (!(pl_FD=fopen(pl_NomFile,"r+")))
		{
			/*B 
			** Si on n arrive pas a ouvrir le fichier en read-write: sortie en fatal :
			** on est dans l incapacite de gerer la coherence des versions logiciel
			*/
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Init_Tab_Checklist : Impossible d ouvrir le fichier de checklist %s en read-write",pl_NomFile);
			return( XDC_NOK);	
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"cliv_Init_Tab_Checklist : Ouvrir du fichier de travail %s reussi en read-write",pl_NomFile);
			/*B
			** On se positionne au debut du fichier
			*/
			fseek(pl_FD,0L,SEEK_SET);
			vl_indice = 0;
			
			while ((vl_indice < XZCGC_NB_VERIF)&&(vl_retcode != EOF))
			{
				vl_retcode = fscanf(pl_FD,"%d %d",
						&(CLIVG_Tab_Checklist[vl_indice].TypeVerif),
						&(CLIVG_Tab_Checklist[vl_indice].Flag));
				vl_indice++;
			}
			if ( vl_indice != XZCGC_NB_VERIF)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Init_Tab_Checklist : Problème de lecture sur le fichier %s\n Suggestion : rm %s",pl_NomFile,pl_NomFile);
				fclose(pl_FD);
				return( XDC_NOK);
			}
		}		
	}
	/*A
	** Initialise la globale permettant d acceder au fichier de checklist.
	*/		
	CGCDG_Fich_CHECKLIST = pl_FD;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_Init_Tab_Checklist : Fin d execution : OK" );
	return( XDC_OK);
} 
 
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cliv_Liv_Diff_Fichier : permet d effectuer la commande de livraison ou de diffusion
*  et de prevenir en cas de problème l administrateur.
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cliv_Liv_Diff_Fichier(	char * va_Liv_ou_Diff,
				char * va_TypeFich,
				char * va_NomFichier,
				char * va_Rep_Source,
				char * va_Machine_Source,
				char * va_Rep_Dest,
				char * va_Machine_Dest)
/*
* ARGUMENTS EN ENTREE : 	
* ARGUMENTS EN SORTIE :
* CODE RETOUR : 
*	XDC_OK si le type de fichier est correct 
*	sinon XDC_NOK
* CONDITION D'UTILISATION   
* FONCTION 
------------------------------------------------------*/
{
	static char *version 		= "$Id: cgcd_liv.c,v 1.16 2018/10/04 10:22:50 devgfi Exp $ : cliv_Liv_Diff_Fichier";
	int	vl_retcode		= XDC_OK;
	int	vl_numerr		= 0;
	char 	vl_mesgerr[80]		= "";
	char 	vl_line	[500]		= "";
	char 	vl_commande[500]	= "";
	char 	vl_Msg[500]		= "";
	int	vl_Etat			= 0;
	FILE *	vl_ShellD		= NULL;
	int	pl_Etat			= XDC_OK;
	int	vl_Status		= XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN   : cliv_Liv_Diff_Fichier a recu : \n Type de commande %s\n Type de Fichier %s\n Nom du Fichier %s Rep Source %s\n De la machine source %s\n vers le rep Destination %s \nsur la machine %s \n",
				va_Liv_ou_Diff,
				va_TypeFich,
				va_NomFichier,
				va_Rep_Source,
				va_Machine_Source,
				va_Rep_Dest,
				va_Machine_Dest );
	
	if(!strcmp(va_Liv_ou_Diff,CLIVC_LIVRAISON))
	{
		if((vl_retcode = cliv_Modif_Tab_Checklist(va_TypeFich,XDC_NOK))==XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					va_TypeFich,XDC_NOK); 
		}
		/*B
		** si on livre un fichier qui permet de modifier la base, on impose la sauvegarde
		** avant d effectuer la verification de coherence
		*/
		if ( (strcmp(va_TypeFich, XZCGC_TYPEF_DORA) == 0)  ||  (strcmp(va_TypeFich, XZCGC_TYPEF_EQUI) == 0))
		{
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_TRANSFERT_BASE,XDC_NOK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_TRANSFERT_BASE,XDC_NOK);
			}
		}
	}
	else
	{
		if((vl_retcode = cliv_Consult_Tab_Checklist(va_TypeFich, &vl_Etat))!=XDC_OK)
		{		
			XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : %s : cliv_Consult_Tab_Checklist a echoue",va_Liv_ou_Diff);
			sprintf(vl_Msg,"%s non accordée sur Type de Fichier %s vers rep %s de %s",
				va_Liv_ou_Diff,
				va_TypeFich,
				va_Rep_Dest,
				va_Machine_Dest );
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cliv_Liv_Diff_Fichier : Erreur" );
			return(XDC_NOK);
		}
		else
		{	/*B
			** Test si on accorde la diffusion
			*/
			if ((pl_Etat == XDC_OK ) || (CGCDV_CHECKLIST == XDC_NOK))
			{
				sprintf(vl_Msg,"%s accordée sur Type de Fichier %s vers rep %s de %s",
					va_Liv_ou_Diff,
					va_TypeFich,
					va_Rep_Dest,
					va_Machine_Dest );
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"TCGCD : %s ",vl_Msg);
			}
			else
			{	
				sprintf(vl_Msg,"%s non accordée sur Type de Fichier %s vers rep %s de %s : effectuez les vérifications de cohérence sur %s",
					va_Liv_ou_Diff,
					va_TypeFich,
					va_Rep_Dest,
					va_Machine_Dest,
					va_TypeFich );
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : %s ",vl_Msg);
			
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cliv_Liv_Diff_Fichier : Erreur" );
				return(XDC_NOK);
			}
		}			
	}
	/*A
	** On regarde si le fichier est global : livraison par repertoire.
	*/
	if(!strcmp(va_NomFichier,XZCGC_NOMF_GLOBAL))
	{
		/*A
		** Livraison ou diffusion globale du repertoire
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			va_Rep_Source, 
		 			va_Machine_Dest,  
		 			va_Rep_Dest);
		sprintf(vl_Msg,"Demande de %s de %s sur %s de la machine %s",
					va_Liv_ou_Diff,
					va_Rep_Source,
					va_Rep_Dest,
					va_Machine_Dest);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		vl_Status = XDC_OK;
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur système : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
			
		}
		else
		{
			sprintf(vl_Msg,"%s : le rep %s n a pas été livré sur %s : l'éxécution de la commande %s a echoué",
				va_Liv_ou_Diff,
				va_Rep_Source,
				va_Machine_Dest,
				vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier : Problème d execution de la commande %s",vl_commande);	
		}
		pclose(vl_ShellD);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cliv_Liv_Diff_Fichier sortie avec %d",vl_Status );
		return(vl_Status);	
	}
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");
	sprintf(vl_Msg,"Demande de %s de %s du rep %s de la machine %s dans %s de la machine %s",
				va_Liv_ou_Diff,
				va_NomFichier,
				va_Rep_Source,
				va_Machine_Source,
				va_Rep_Dest,
				va_Machine_Dest);
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);			
				
	if((vl_retcode=XZSS_07TransfertFichier (va_Machine_Source,
						va_NomFichier,
						va_Rep_Source,
						va_Machine_Dest,
						va_NomFichier,
						va_Rep_Dest))!=XDC_OK)
	{
		/*B
		** Previent l administrateur du prob survenu.
		*/
		cliv_Inform_Admin_Probl(	va_Liv_ou_Diff,
						va_NomFichier,
						va_Rep_Source,
						va_Machine_Source,
						va_NomFichier,
						va_Rep_Dest,
						va_Machine_Dest,
						vl_retcode);
			
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cliv_cb : Sortie en erreur.");
		return(XDC_NOK);
	}
	else
	{
		sprintf(vl_Msg,"%s : le fichier %s de %s a été livré sur %s dans %s",
				va_Liv_ou_Diff,
				va_NomFichier,
				va_Rep_Source,
				va_Machine_Dest,
				va_Rep_Dest);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cliv_Liv_Diff_Fichier" );
	return(XDC_OK);
}

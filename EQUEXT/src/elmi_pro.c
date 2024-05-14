/*E*/
/* Fichier : $Id: elmi_pro.c,v 1.25 2011/02/25 10:26:05 gesconf Exp $        Release : $Revision: 1.25 $        Date : $Date: 2011/02/25 10:26:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER elmi_pro.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de gestion du protocole LCR maitre de la tache TELMi.
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	26 Oct 1994	: Creation
* Flaidar.A	version 1.2	01 Dec 1994	: Ajout verif. malloc O.K.
* Mismer.D	version 1.3	22 Dec 1994	:
* Mismer.D	version 1.4	13 Jan 1995	:
* Volcic.F	version 1.5	13 Jan 1995	:
* Mismer.D	version 1.6	26 Jan 1995	:
* Mismer.D	version 1.7	09 Fev 1995	:
* Mismer.D	version 1.8	08 Mar 1995	:
* Volcic.F	version 1.9	19 Avr 1995	:
* Mismer.D	version 1.10	16 Mai 1995	: Modif envoi mise a l'heure et traitement defaut ST
* Fontaine.C	version 1.11	23 Mai 1995	: Reprise historique
* Mismer.D	version 1.12	13 Jun 1995	: Modif reconnexion RTserver
* Volcic.F	version 1.13	28 Jul 1995	: Modif trace detection message
* Mismer.D  	version 1.14	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.15	02 Fev 1996	: Modif mise a l'heure equipement
* Mismer.D  	version 1.16	09 Mai 1996	: Modif time-out reponse equipement pour modif idletimer sur transerver (DEM/1111)
* Mismer.D  	version 1.17	19 Nov 1996	: Modif pour ajout tache METEO (DEM/1232)
* Mismer.D	version 1.18 	07 Jan 1997	: Ajout reponse commande date (DEM/1155)
* 			        25 Mar 1997     : Ajout date pour TDP (DEM/1396)
* Mismer.D	version 1.19 	07 Nov 1997	: Modif pour trace detection message errone (DEM/xxxx)
* Niepceron	v1.22		juin 2008	: Modif traces
* JMG		V1.23		22/06/10	: linux
* JMG		V1.24		22/02/11	: linux
* JPL		25/02/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,*)  1.25
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "ex_mrtf.h"

#include "elmi_cfg.h"
#include "elmi_pro.h"

/* definitions de constantes */

#define CM_VAL_DEFAUT_TIMER 4.0

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: elmi_pro.c,v 1.25 2011/02/25 10:26:05 gesconf Exp $      : elmi_pro";

/* declaration des variables externes */

/* definition de fonctions internes */

void epro_envoi_trame_LCR(XZEXT_Cmde_LCR *);
int epro_traitement_msg_lser(XZEXT_Cmde_LCR *, XZEXT_Msg_Socket *,
							 EPROT_Conditions *, XZEXT_Msg_TEDI *);
void epro_traitement_msg_donn(XZEXT_Msg_Socket *);

/* declaration de fonctions externes */
extern void edir_traitement_dcnx();
extern int edir_detecte_type_sock(void);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Envoi d'une trame TEDI/LCR suite a une demande d'un client TEzzz
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_envoi_trame_LCR(  XZEXT_Cmde_LCR *pa_CmdeLCR )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a envoyer.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde. LCR a envoyer.
*
* FONCTION
*
* - envoi trame LCR et attente reponse
* - empilement des eventuelles demandes des clients TEzzz ds. une liste lors
*   de l'attente de la reponse au mesage TEDI/LCR
*
------------------------------------------------------*/
void epro_envoi_trame_LCR(XZEXT_Cmde_LCR *pa_CmdeLCR)
{
	static char *version = "$Id: elmi_pro.c,v 1.25 2011/02/25 10:26:05 gesconf Exp $      : epro_envoi_trame_LCR ";

	XZEXT_Cmde_LCR vl_CmdeLCR;		   /* structure depouillement messages a empiler */
	XZEXT_Comp_Trame_TEDI vl_SkelTEDI; /* composants trame TEDI pour EX_encapsuler_LCR */
	XZEXT_Msg_TEDI vl_MsgTEDI;		   /* contient le msg. TEDI a envoyer ET sa long. */
	XDY_Texte vl_TypeMsg;			   /* juste pour recuperer le type de message */
	EPROT_Conditions vl_Cond;		   /* conditions d'envoi trame : 	nbre. essais,
										   nbre. erreurs,
										   nbre. non reponses,
						   */
	XZEXT_Msg_Socket vl_MsgSock;	   /* message lu localement */
	int vl_EtatCnx;					   /* etat conexion socket active apres lecture */
	int vl_AttenteRep;				   /* XDC_VRAI s'il faut attendre la rep. de l'equip. */
	XZEXT_MASK vl_ElmSockList;		   /* pour l'insertion sock. RTServer ds. la liste */
	XZEXT_MASK *pl_ListSock = NULL;
	char *pl_TrameLisible;		/* pointeur sur trame format lisible */
	XZSMT_Horodate vl_Horodate; /* horodate en format lisible */
	XDY_Horodate vl_HorodateSec;
	XZSMT_Horodate vl_Horodatedecompo;
	int vl_JourSem; /* jour de la semaine */
	int vl_retour = 0;

	if (vg_EtatST != XDC_OK)
	{
		/*A formatage reponse */
		sprintf(vl_MsgSock.msg, "rep. telmi %s 0 0 0 %s %d %d %s %s %s",
				ecom_lire_horodate(&vl_Horodate),
				pa_CmdeLCR->rgs,
				pa_CmdeLCR->type_cmd,
				XZEXC_TRAME_NON_TRANSM,
				XZEXC_SEP_TRAME,
				pa_CmdeLCR->cmd_LCR,
				XZEXC_FIN_MSG_SOCK);
		vl_MsgSock.len = (int)strlen(vl_MsgSock.msg);
		XZST_03EcritureTrace(XZSTC_WARNING, "epro_envoi_trame_LCR : Generation message <%s>", vl_MsgSock.msg);
		/*A emission reponse vers la tache TEzzz client */
		/*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
		//sem_wait(&vg_semaphore);
		if (write(pa_CmdeLCR->sock_clnt, vl_MsgSock.msg, vl_MsgSock.len) != vl_MsgSock.len)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "epro_envoi_trame_LCR : Emission message <%s> vers client TEzzz impossible.",
								 vl_MsgSock.msg);
		}
		//sem_post(&vg_semaphore);
		return;
	}

	vl_AttenteRep = XDC_VRAI;

	/*A mise a jour composants trame TEDI */
	/*A mise a jour type message TEDI */
	vl_SkelTEDI.type_msg = pa_CmdeLCR->type_msg; /* type msg. TEDI associe a la cmde. LCR */

	/*A mise a jour adresse rgsb du message en fonction de son type */
	/*A CAS type message */
	switch (vl_SkelTEDI.type_msg)
	{
	/*A cas XZEXC_MSG_REQ */
	case XZEXC_MSG_REQ:
		/* Cas particulier de la date */
		XZST_03EcritureTrace(XZSTC_DEBUG1, "XZEXC_MSG_REQ");
		/* Envoi de la date sous format normal DT ID= JJ/MM/AA HH:MM:SS */
		if (pa_CmdeLCR->type_cmd == XZEXC_TRAME_DATE)
		{
			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec);
			/*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
			XZSM_11ConversionHorodate(vl_HorodateSec, &vl_Horodatedecompo);
			/* passage de l'annee sur 2 digit */
			memcpy(&vl_Horodatedecompo[6], &vl_Horodatedecompo[8], 12);

			/*B Composition de la trame mise a la date et a l'heure */
			sprintf(pa_CmdeLCR->cmd_LCR, "DT ID=%s %s", vg_idpassw, vl_Horodatedecompo);
			XZST_03EcritureTrace(XZSTC_DEBUG2, "trame de date:<%s0 %s>", pa_CmdeLCR->rgs, pa_CmdeLCR->cmd_LCR);
		}

		/* Envoi le prefixe ID avant la commande de date */
		if ((pa_CmdeLCR->type_cmd == XZEXC_TRAME_DATE_ID_DATE) ||
			(pa_CmdeLCR->type_cmd == XZEXC_TRAME_DATE_ID_DT))
		{
			strcpy(pa_CmdeLCR->cmd_LCR, "ID");
			memcpy(&vl_CmdeLCR, pa_CmdeLCR, sizeof(XZEXT_Cmde_LCR));
			vl_CmdeLCR.type_cmd = XZEXC_TRAME_DATE_ID_SUITE;
			sprintf(vl_CmdeLCR.cmd_LCR, "%s\0", (pa_CmdeLCR->type_cmd == XZEXC_TRAME_DATE_ID_DT) ? "DT" : "DATE");
			ecom_insertfirst_cmd_LCR(&vl_CmdeLCR, &pg_ListeCmdesLCR);
			XZST_03EcritureTrace(XZSTC_DEBUG2, "trame de date:<%s0 %s>", pa_CmdeLCR->rgs, pa_CmdeLCR->cmd_LCR);
		}

		/* Envoi de la date sous format  DT ou DATE JJ/MM/AA HH:MM:SS */
		if (pa_CmdeLCR->type_cmd == XZEXC_TRAME_DATE_ID_SUITE)
		{
			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec);
			/*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
			XZSM_11ConversionHorodate(vl_HorodateSec, &vl_Horodatedecompo);
			/* passage de l'annee sur 2 digit */
			memcpy(&vl_Horodatedecompo[6], &vl_Horodatedecompo[8], 12);

			/*B Composition de la trame mise a la date et a l'heure */
			sprintf(pa_CmdeLCR->cmd_LCR, "%s %s", pa_CmdeLCR->cmd_LCR, vl_Horodatedecompo);
			XZST_03EcritureTrace(XZSTC_DEBUG2, "trame de date:<%s0 %s>", pa_CmdeLCR->rgs, pa_CmdeLCR->cmd_LCR);
		}

		/*B mise a jour adresse rgsb et trame_LCR ds. vl_SkelTEDI */
		sprintf(vl_SkelTEDI.adr_RGSB, "%s%d", pa_CmdeLCR->rgs, XZEXC_NOBL_REQ_TEDI);
		strcpy(vl_SkelTEDI.trame_LCR, pa_CmdeLCR->cmd_LCR);
		/*B SI requette avec adresse rgs contient un car. '0' */
		if (strchr(pa_CmdeLCR->rgs, XZEXC_CAR_JOKER) != NULL)
		{
			/*B ALORS pas d'attente reponse equip. */
			vl_AttenteRep = XDC_FAUX;
		}

		break;

	/*A cas XZEXC_MSG_INFO ou XZEXC_BLC_INFO */
	case XZEXC_MSG_INFO:
	case XZEXC_BLC_INFO:
		XZST_03EcritureTrace(XZSTC_DEBUG1, "XZEXC_MSG_INFO");
		/*B mise a jour adresse rgsb et trame_LCR ds. vl_SkelTEDI */
		strcpy(vl_SkelTEDI.trame_LCR, pa_CmdeLCR->cmd_LCR);
		sprintf(vl_SkelTEDI.adr_RGSB, "%s%s", pa_CmdeLCR->rgs, pa_CmdeLCR->ack);
		break;

	/*A cas XZEXC_MSG_ACQUIT */
	default:
		/*B mise a jour adresse rgsb ds. vl_SkelTEDI */
		XZST_03EcritureTrace(XZSTC_DEBUG1, "XZEXC_MSG_ACQUIT");
		sprintf(vl_SkelTEDI.adr_RGSB, "%s", pa_CmdeLCR->ack);
		break;
	}
	/* FIN CAS */

	/*A mise a jour nbre. cars. pref. & suff. */
	vl_SkelTEDI.long_pref = vg_Config.ligne_serie.NbCarPref;
	vl_SkelTEDI.long_suff = vg_Config.ligne_serie.NbCarSuff;

	/*A mise a jour conds. essais envoi trame TEDI/LCR */
	if ((pa_CmdeLCR->type_cmd == XZEXC_TRAME_TEST_COM) || (pa_CmdeLCR->type_cmd == XZEXC_TRAME_FEN_LCR))
		vl_Cond.nb_essais = 1;
	else
		vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
	vl_Cond.nb_err_crc = 0;
	vl_Cond.nb_non_rep = 0;

	/*A encapsulation msg. TEDI/LCR -> appel EX_encapsuler_LCR() */
	if (pa_CmdeLCR->type_cmd == XZEXC_TRAME_FEN_LCR)
	{
		for (pl_ListSock = vg_SockList; pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant)
			if (pl_ListSock->desc_sock == pa_CmdeLCR->sock_clnt)
			{
				if (strncmp(pl_ListSock->adresse, XZEXC_ADR_SOCK_GTCTELM, strlen(XZEXC_ADR_SOCK_GTCTELM)))
					vl_SkelTEDI.trame_LCR[strlen(vl_SkelTEDI.trame_LCR) - 1] = '\0';
				break;
			}
	}
	/*A SI encapsulation echoue ALORS ecriture trace :"Probleme encapsulation msg. TEDI..." et RETOUR */
	if (EX_encapsuler_LCR(&vl_SkelTEDI, &vl_MsgTEDI) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Probleme encapsulation msg. %s...", vl_SkelTEDI.trame_LCR);
		return;
	}

	/*A
	 * sequence envoi message TEDI/LCR sur la ligne serie geree
	 */

	/*A TANT QUE essais encore a faire */
	while (vl_Cond.nb_essais > 0)
	{
		/*B decrementer nbre. essais */
		vl_Cond.nb_essais--;

		/*B Init. valeur timer attente reponse sur ligne serie */
		vl_Cond.val_timer = CM_VAL_DEFAUT_TIMER;

		/*B emission message sur socket connex. serveur terminaux - ligne serie geree */
		/*B SI emission echoue ALORS ecriture trace : "Emission message TEDI/LCR vers serv. terms. impossible "*/
		//sem_wait(&vg_semaphore);
		if (write(vg_DesSocks.lser, vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len) != vl_MsgTEDI.len)
		{
			//sem_post(&vg_semaphore);
			if ((pl_TrameLisible = malloc(4 * (size_t)vl_MsgTEDI.len + 1)))
			{
				XZST_03EcritureTrace(XZSTC_INFO, "epro_envoi_trame_LCR : Emission message TEDI/LCR <%s> vers serv. terms. impossible ",
									 ecom_rendre_trame_lisible(vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible));
				free(pl_TrameLisible);
			}
			/*B mise timer a XZEXC_TIMER_ECOULE pour relancer un nouvel essai */
			vl_Cond.val_timer = XZEXC_TIMER_ECOULE;
			/*B incremernter nbr. non rep */
			vl_Cond.nb_non_rep++;
		}
		else
		{
			//sem_post(&vg_semaphore);
			if ((pl_TrameLisible = malloc(4 * (size_t)vl_MsgTEDI.len + 1)))
			{
				XZST_03EcritureTrace(XZSTC_INFO, "Emission message %s (%d) TEDI/LCR <%s> vers serv. terms. ", pa_CmdeLCR->rgs, vl_Cond.nb_essais,
									 ecom_rendre_trame_lisible(vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible));
				free(pl_TrameLisible);
			}
		}

		/*B TANT QUE timer non ecoule */
		while (vl_Cond.val_timer > XZEXC_TIMER_ECOULE)
		{

			/*B SI pas d'attente reponse ALORS return */
			if (vl_AttenteRep == XDC_FAUX)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : pas d'attente de reponse de l'equipement");
				return;
			}
			/*B SINON */
			else
			{
				/*B attente avec TIMER d'un evnmt. sur la liste des sockets */
				/*B SI attente echoue */
				XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : Avant ex_attendre_msg_ST vl_Cond.val_timer=%lf",vl_Cond.val_timer);
				if (ex_attendre_msg_ST(XZEXC_SOCKET_NULL, &vg_SockList,
									   vl_Cond.val_timer, &vg_DesSocks.curr,
									   &vl_Cond.val_timer) != XDC_OK)
				{
					/*B ALORS ecriture trace :  "epro_envoi_trame_LCR : Probleme attente message avec timer sur socket(s)." */
					XZST_03EcritureTrace(XZSTC_WARNING, " Probleme attente message avec timer sur socket(s).");
					ex_ConnexionSocketRTserver(&vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList);
				}
				/*B SINON */
				else
				{
					XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : Apres ex_attendre_msg_ST vl_Cond.val_timer=%lf",vl_Cond.val_timer);
					ex_ConnexionSocketRTserver(&vg_DesSocks.rtsrv, 0, &vg_SockList);

					/*B SI socket active = socket RTServer */
					if (vg_DesSocks.curr == vg_DesSocks.rtsrv)
					{
						/*B ALORS appel primitive standard lecture RTWorks */
					}
					/*B SINON SI evenement sur une socket diff. de la socket d'ecoute
					 *  (si. evt. sur socket ecoute <=> cnx. d'un client geree par ex_attendre_msg())
					 *  tout est O.K. => rien a faire */
					else if (vg_DesSocks.curr == vg_DesSocks.ect) /* cas connexion client */
					{
					}
					/*B SINON SI aucun evenement <=> timer ecoule ! */
					else if (vg_DesSocks.curr == XZEXC_NO_EVT)
					{
						/*B ALORS incrementer nbre; non reponses */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : vg_DesSocks.curr == XZEXC_NO_EVT");
						vl_Cond.nb_non_rep++;
					}
					/*B SINON O.K., socket active soit lser, soit donnees */
					else
					{
						/*B detection adresse socket active */
						ecom_detecte_clnt(&vg_DesSocks, &vg_SockList);

						XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : Avant ex_lire_msg");
						/*B SI lecture sur socket active echoue */
						if ((vl_retour = ex_lire_msg(vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len,
													 &vg_SockList, &vl_EtatCnx)) != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : vl_retour == XZEXC_NO_EVT");
							if (vl_retour == XZEXC_NO_EVT)
							{
								vl_Cond.nb_non_rep++;
							}
							else
							{
								/*B ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
								XZST_03EcritureTrace(XZSTC_WARNING, "epro_envoi_trame_LCR : Probleme lecture socket adresse %s...",
													 vg_DesSocks.curr_addr);
								vg_EtatST = XDC_NOK;
							}
						}
						/*B SINON SI deconnexion d'un client ALORS appel edir_traitement_dcnx() */
						else if (vl_EtatCnx == XZEXC_DECONNEC)
						{
							edir_traitement_dcnx();
						}
						/*B SINON detection type socket active et traitement message recu */
						else
						{
							XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : traitement message recu");
							/*B CAS type de socket active */
							switch (edir_detecte_type_sock())
							{
							/*B cas : XZEXC_SOCK_ACT_LSER : sock. ligne serie active */
							case XZEXC_SOCK_ACT_LSER:
								/*B appel epro_traitement_msg_lser() */
								/*B SI traitement msg. recu sur ligne serie OK, ALORS return */
								XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : Avant epro_traitement_msg_lser");
								if (epro_traitement_msg_lser(pa_CmdeLCR, &vl_MsgSock,
															 &vl_Cond, &vl_MsgTEDI) == XDC_OK)
								{
									XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : Apres epro_traitement_msg_lser : return");
									return;
								}
								/*B SINON ereur TEDI ou CRC detectee, mise timer a zero */
								vl_Cond.val_timer = XZEXC_TIMER_ECOULE;
								XZST_03EcritureTrace(XZSTC_DEBUG1, "epro_envoi_trame_LCR : Apres epro_traitement_msg_lser : vl_Cond.val_timer = XZEXC_TIMER_ECOULE / break");
								break;

								/*B cas : XZEXC_SOCK_ACT_DONN : sock. donnees client active */
							case XZEXC_SOCK_ACT_DONN:
								/*B appel epro_traitement_msg_donn() */
								epro_traitement_msg_donn(&vl_MsgSock);
								break;

								/*B cas defaut : XZEXC_SOCK_ACT_UNDEF : sock. ecoute active = cnx. nouveau clnt. */
							default:
								break;
							}
							/*B FIN CAS */
						}
						/*B FIN SI lecture SINON ... */
					}
					/*B FIN SI socket RTServer SINON SI ...*/
				}
				/*B FIN SI attente SINON ... */
			}
			/*B FIN SI emission message SINON ... */

			if (vg_EtatST != XDC_OK)
			{
				vl_Cond.val_timer = XZEXC_TIMER_ECOULE;
				vl_Cond.nb_essais = 0;
			}
		}
		/*B FIN TANT QUE timer non ecoule */
	}
	/*A FIN TANT QUE NbEssais > 0 */

	/*A Nbre. d'essai epuise => envoi reponse CMDE. LCR NON TRANSMISE a la tache cliente TEzzz */
	/*A formatage reponse */
	sprintf(vl_MsgSock.msg, "rep. telmi %s %d %d %d %s %d %d %s %s %s",
			ecom_lire_horodate(&vl_Horodate),
			vl_Cond.nb_non_rep,
			vl_Cond.nb_err_crc,
			pa_CmdeLCR->no_cmd,
			pa_CmdeLCR->rgs,
			pa_CmdeLCR->type_cmd,
			XZEXC_TRAME_NON_TRANSM,
			XZEXC_SEP_TRAME,
			pa_CmdeLCR->cmd_LCR,
			XZEXC_FIN_MSG_SOCK);
	vl_MsgSock.len = (int)strlen(vl_MsgSock.msg);

	/*A emission reponse vers la tache TEzzz client */
	/*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
	//sem_wait(&vg_semaphore);
	if (write(pa_CmdeLCR->sock_clnt, vl_MsgSock.msg, vl_MsgSock.len) != vl_MsgSock.len)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "epro_envoi_trame_LCR : Emission message <%s> vers client TEzzz impossible.",
							 vl_MsgSock.msg);
	}
	//sem_post(&vg_semaphore);
	/*B FIN SI */
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Traitement d'un message recu sur la socket comms. avec le serveur de terminaux.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  int epro_traitement_msg_lser(  XZEXT_Cmde_LCR *pa_CmdeLCR ,
*				XZEXT_Msg_Socket *pa_MsgSock,
*				EPROT_Conditions *pa_Cond,
*				XZEXT_Msg_TEDI  *pa_MsgTEDI )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR dont on recoit la reponse sur la ligne serie.
* pa_MsgSock	: pointeur sur le message recu sur la socket comms; avec la ligne serie geree.
* pa_Cond	: pointeur sur la structure des conds. pour gerer le protocole.
* pa_MsgTEDI	: pointeur sur le message TEDI a envoyer en cas d'erreur CRC bloc intermed.
*
* ARGUMENTS EN SORTIE :
*
* pa_Cond	: les conditions mises a jour en fonction de la trame TEDI/LCR recue.
* pa_MsgTEDI	: msg. TEDI (acquit. neg. a envoyer en cas de err. CRC blc. interm.
*
* CODE RETOUR :
*
* XDC_OK	: si pas d'erreur CRC ou TEDI detectee.
* XDC_NOK	: sinon.
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde. LCR dont on recoit la reponse.
* pa_MsgSock doit contenir la reponse a pa_CmdeLCR.
*
* FONCTION
*
* - detection type reponse ( message bloc unique, bloc intermediaire, aquittement ).
* - si c'est le cas, envoi compte rendu a la atche TEzzz ayant envoye pa_CmdeLCR.
*
------------------------------------------------------*/
int epro_traitement_msg_lser(XZEXT_Cmde_LCR *pa_CmdeLCR,
							 XZEXT_Msg_Socket *pa_MsgSock,
							 EPROT_Conditions *pa_Cond,
							 XZEXT_Msg_TEDI *pa_MsgTEDI)
{
	static char *version = "$Id: elmi_pro.c,v 1.25 2011/02/25 10:26:05 gesconf Exp $      : epro_traitement_msg_lser";

	char vl_NoBloc;					   /* no. bloc du message recu */
	char vl_DDM[5];					   /* debut du message reponse atendu */
	XZEXT_Cmde_LCR vl_CmdeLCR;		   /* cmde LCR a empiler ds. la liste */
	XZEXT_Msg_Socket vl_MsgSock;	   /* message comms. sur socket */
	int vl_ResVerif;				   /* resultat de la verification TEDI/LCR */
	int vl_TypeRep;					   /* type de la reponse TELLMi pour TEzzz */
	XZEXT_Comp_Trame_TEDI vl_SkelTEDI; /* composants trame TEDI pour EX_encapsuler_LCR */
	char *pl_TrameLisible;			   /* pointeur sur trame lisible */
	XZSMT_Horodate vl_Horodate;		   /* horodate en format converti */

	XDY_Texte vl_Texte;

	vl_Texte[0] = '\0';

	/*B mise a jour composant trame TEDI pour un eventuel NAK a envoyer */
	vl_SkelTEDI.long_pref = vg_Config.ligne_serie.NbCarPref;
	vl_SkelTEDI.long_suff = vg_Config.ligne_serie.NbCarSuff;

	/*A creation DDM attendu en debut de reponse */
	sprintf(vl_DDM, "%c%s", XZEXC_CAR_STX, pa_CmdeLCR->rgs);

	/*A on s'attend aussi qu'on acquitte avec no. bloc = XZEXC_NOBL_REQ_TEDI */
	vl_NoBloc = XZEXC_CAR_NO_BLC_REQ;
	XZST_03EcritureTrace(XZSTC_DEBUG1, "MsgSock=%s", pa_MsgSock);
	/*A CAS resultat de la verification de la trame recue */
	switch (vl_ResVerif = ex_verif_TEDI_CRC(pa_MsgSock, vl_DDM, &vl_NoBloc))
	{
	case XZEXC_BLC_OK: /*A cas bloc. interm. OK */

		sprintf(vl_Texte, "detection bloc intermed. %s %c OK, bloc attendu %c", pa_CmdeLCR->rgs, vl_NoBloc, pa_CmdeLCR->no_blc_suivant);
		XZST_03EcritureTrace(XZSTC_INFO, "epro_traitement_msg_lser : %s %s\n",
							 vl_Texte, pa_MsgSock->msg);

		/*B SI no. bloc recu diff. du no. blc. attendu */
		if (vl_NoBloc != pa_CmdeLCR->no_blc_suivant)
		{
			/*B ALORS encapsuler trame TEDI a envoyer = acquit. neg. du bloc attendu */
			sprintf(vl_SkelTEDI.adr_RGSB, "%c%c", XZEXC_CAR_NAK, pa_CmdeLCR->no_blc_suivant);
			vl_SkelTEDI.type_msg = XZEXC_MSG_ACQUIT;
			if (EX_encapsuler_LCR(&vl_SkelTEDI, pa_MsgTEDI) != XDC_OK)
			{
				if ((pl_TrameLisible = malloc(4 * strlen(vl_SkelTEDI.adr_RGSB) + 1)))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : Probleme encapsulation TEDI msg. <%s>",
										 ecom_rendre_trame_lisible(vl_SkelTEDI.adr_RGSB, 2, pl_TrameLisible));
					free(pl_TrameLisible);
				}
			}

			/*B incrementer nbre. erreurs CRC */
			pa_Cond->nb_err_crc++;

			/*B return XDC_NOK */
			return (XDC_NOK);
		}

		/*B type de reponse TELMi = XZEXC_REP_BLC_INT */
		vl_TypeRep = XZEXC_REP_BLC_INT;

		/*B remplissage composants cmde. acquittement a empiler avec les caracteristiques de la
		 *  demande qui a genere l'echange en blocs et le no. du bloc suivant a attendre */
		vl_CmdeLCR = *pa_CmdeLCR;
		vl_CmdeLCR.no_blc_suivant = (pa_CmdeLCR->no_blc_suivant == '9' ? '0' : pa_CmdeLCR->no_blc_suivant + (char)1);

		/*B il s'agit d'un acquit. => mise a jour des champs specifiques : ack = [ACK]b et type_msg = XZEXC_MSG_ACQUIT */
		sprintf(vl_CmdeLCR.ack, "%c%c", XZEXC_CAR_ACK, vl_NoBloc);
		vl_CmdeLCR.type_msg = XZEXC_MSG_ACQUIT;

		/*B empilement acquitt. en tete de liste des cmdes LCR a traiter */
		ecom_insertfirst_cmd_LCR(&vl_CmdeLCR, &pg_ListeCmdesLCR);

		break;

	case XZEXC_MSG_OK: /*A cas message bloc unique OK */

		sprintf(vl_Texte, "detection bloc unique. %s %c OK", pa_CmdeLCR->rgs, vl_NoBloc);
		XZST_03EcritureTrace(XZSTC_INFO, "epro_traitement_msg_lser : %s %s\n",
							 vl_Texte, pa_MsgSock->msg);

		/*B SI le dernier bloc d'un echange et le no. bloc diff. du no. bloc attendu */
		if (pa_CmdeLCR->type_msg == XZEXC_MSG_ACQUIT && pa_CmdeLCR->no_blc_suivant != vl_NoBloc)
		{
			/*B ALORS encapsuler trame TEDI a envoyer = acquit. neg. du bloc attendu */
			sprintf(vl_SkelTEDI.adr_RGSB, "%c%c", XZEXC_CAR_NAK, pa_CmdeLCR->no_blc_suivant);
			vl_SkelTEDI.type_msg = XZEXC_MSG_ACQUIT;
			if (EX_encapsuler_LCR(&vl_SkelTEDI, pa_MsgTEDI) != XDC_OK)
			{
				if ((pl_TrameLisible = malloc(4 * strlen(vl_SkelTEDI.adr_RGSB) + 1)))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : Probleme encapsulation msg. TEDI...",
										 ecom_rendre_trame_lisible(vl_SkelTEDI.adr_RGSB, 2, pl_TrameLisible));
					free(pl_TrameLisible);
				}
			}

			/*B incrementer nbre. erreurs CRC */
			pa_Cond->nb_err_crc++;

			/*B return XDC_NOK */
			return (XDC_NOK);
		}

		/*B SI res. verif. TEDI/LCR = XZEXC_MSG_OK ALORS type reponse TELMi = XZEXC_REP_BLC_FIN */
		vl_TypeRep = XZEXC_REP_BLC_FIN;

		break;

	case XZEXC_ACK_OK: /*A cas acquittement positif OK */

		strcpy(vl_Texte, "detection acquittement positif OK");
		XZST_03EcritureTrace(XZSTC_INFO, "epro_traitement_msg_lser : %s %s\n",
							 vl_Texte, pa_MsgSock->msg);

		/*B SI acquitt. recu alors qu'on attendait un bloc intermed.  */
		if (pa_CmdeLCR->type_msg == XZEXC_MSG_ACQUIT)
		{
			/*B ALORS encapsuler trame TEDI a envoyer = acquit. neg. du bloc attendu */
			sprintf(vl_SkelTEDI.adr_RGSB, "%c%c", XZEXC_CAR_NAK, pa_CmdeLCR->no_blc_suivant);
			vl_SkelTEDI.type_msg = XZEXC_MSG_ACQUIT;
			if (EX_encapsuler_LCR(&vl_SkelTEDI, pa_MsgTEDI) != XDC_OK)
			{
				if ((pl_TrameLisible = malloc(4 * strlen(vl_SkelTEDI.adr_RGSB) + 1)))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : Probleme encapsulation msg. TEDI...",
										 ecom_rendre_trame_lisible(vl_SkelTEDI.adr_RGSB, 2, pl_TrameLisible));
					free(pl_TrameLisible);
				}
			}

			/*B incrementer nbre. erreurs CRC */
			pa_Cond->nb_err_crc++;

			/*B return XDC_NOK */
			return (XDC_NOK);
		}

		/*B SI res. verif. TEDI/LCR = XZEXC_ACK_OK ALORS type reponse TELMi = XZEXC_REP_ACK */
		vl_TypeRep = XZEXC_REP_ACK;

		break;

	case XZEXC_NAK_OK: /*A cas acquittement negatif OK */

		strcpy(vl_Texte, "detection acquittement negatif OK");
		XZST_03EcritureTrace(XZSTC_INFO, "epro_traitement_msg_lser : %s %s\n",
							 vl_Texte, pa_MsgSock->msg);

		/*B SI acquitt. recu alors qu'on attendait un bloc intermed.  */
		if (pa_CmdeLCR->type_msg == XZEXC_MSG_ACQUIT)
		{
			/*B ALORS encapsuler trame TEDI a envoyer = acquit. neg. du bloc attendu */
			sprintf(vl_SkelTEDI.adr_RGSB, "%c%c", XZEXC_CAR_NAK, pa_CmdeLCR->no_blc_suivant);
			vl_SkelTEDI.type_msg = XZEXC_MSG_ACQUIT;
			if (EX_encapsuler_LCR(&vl_SkelTEDI, pa_MsgTEDI) != XDC_OK)
			{
				if ((pl_TrameLisible = malloc(4 * strlen(vl_SkelTEDI.adr_RGSB) + 1)))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : Probleme encapsulation msg. TEDI...",
										 ecom_rendre_trame_lisible(vl_SkelTEDI.adr_RGSB, 2, pl_TrameLisible));
					free(pl_TrameLisible);
				}
			}

			/*B incrementer nbre. erreurs CRC */
			pa_Cond->nb_err_crc++;

			/*B return XDC_NOK */
			return (XDC_NOK);
		}

		/*B SI res. verif. TEDI/LCR = XZEXC_NAK_OK ALORS type reponse TELMi = XZEXC_REP_NAK */
		vl_TypeRep = XZEXC_REP_NAK;

		break;

	case XZEXC_ERR_CRC_BLC: /*A cas err. CRC bloc intermediaire */

		strcpy(vl_Texte, "detection bloc intermed. erreur CRC");
		XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : %s %s\n", vl_Texte, "PBLM TRACE OK");

		/*A SI demande LCR ayant genere le bloc et non pas un acquit. ALORS return XDC_NOK */
		if (pa_CmdeLCR->type_msg == XZEXC_MSG_REQ)
		{
			return (XDC_NOK);
		}

		/*B encapsuler trame TEDI a envoyer = acquit. neg. du bloc attendu */
		sprintf(vl_SkelTEDI.adr_RGSB, "%c%c", XZEXC_CAR_NAK, pa_CmdeLCR->no_blc_suivant);
		vl_SkelTEDI.type_msg = XZEXC_MSG_ACQUIT;
		if (EX_encapsuler_LCR(&vl_SkelTEDI, pa_MsgTEDI) != XDC_OK)
		{
			if ((pl_TrameLisible = malloc(4 * strlen(vl_SkelTEDI.adr_RGSB) + 1)))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : Probleme encapsulation msg. TEDI...",
									 ecom_rendre_trame_lisible(vl_SkelTEDI.adr_RGSB, 2, pl_TrameLisible));
				free(pl_TrameLisible);
			}
		}

		/*B incrementer nbre. erreurs CRC */
		pa_Cond->nb_err_crc++;

		/*A RETOUR XDC_NOK */
		return (XDC_NOK);

	case XZEXC_ERR_CRC_MSG: /*A cas. err. CRC bloc unique */
		strcpy(vl_Texte, "detection err. CRC message bloc unique");

	case XZEXC_TEDI_NOK: /*A cas. msg. sans structure TEDI */

		if (vl_ResVerif == XZEXC_TEDI_NOK)
			strcpy(vl_Texte, "detection message structure non TEDI");

		XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : %s %s\n", vl_Texte, "PBLM TRACE OK");

		/*B SI on attendait un bloc intermed. */
		if (pa_CmdeLCR->type_msg == XZEXC_MSG_ACQUIT)
		{
			/*B ALORS encapsuler trame TEDI a envoyer = acquit. neg. du bloc attendu */
			sprintf(vl_SkelTEDI.adr_RGSB, "%c%c", XZEXC_CAR_NAK, pa_CmdeLCR->no_blc_suivant);
			vl_SkelTEDI.type_msg = XZEXC_MSG_ACQUIT;
			if (EX_encapsuler_LCR(&vl_SkelTEDI, pa_MsgTEDI) != XDC_OK)
			{
				if ((pl_TrameLisible = malloc(4 * strlen(vl_SkelTEDI.adr_RGSB) + 1)))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_lser : Probleme encapsulation msg. TEDI...",
										 ecom_rendre_trame_lisible(vl_SkelTEDI.adr_RGSB, 2, pl_TrameLisible));
					free(pl_TrameLisible);
				}
			}
		}

		/*B incrementer nbre. erreurs CRC */
		pa_Cond->nb_err_crc++;

		/*A RETOUR XDC_NOK */
		return (XDC_NOK);
	}
	/*A FIN CAS */

	/*B message bien recu, mise a zero du nbre. d'essais */
	pa_Cond->nb_essais = 0;

	/*B formatage message reponse pour la tache TEzzz client */
	if (pa_CmdeLCR->type_cmd != XZEXC_TRAME_DATE)
		sprintf(vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
				XZEXC_REP_TELMi,
				ecom_lire_horodate(&vl_Horodate),
				pa_Cond->nb_non_rep,
				pa_Cond->nb_err_crc,
				pa_CmdeLCR->no_cmd,
				pa_CmdeLCR->rgs,
				pa_CmdeLCR->type_cmd,
				vl_TypeRep,
				XZEXC_SEP_TRAME,
				pa_MsgSock->msg,
				XZEXC_FIN_MSG_SOCK);
	else
		sprintf(vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s~%s %s",
				XZEXC_REP_TELMi,
				ecom_lire_horodate(&vl_Horodate),
				pa_Cond->nb_non_rep,
				pa_Cond->nb_err_crc,
				pa_CmdeLCR->no_cmd,
				pa_CmdeLCR->rgs,
				pa_CmdeLCR->type_cmd,
				vl_TypeRep,
				XZEXC_SEP_TRAME,
				pa_CmdeLCR->cmd_LCR,
				pa_MsgSock->msg,
				XZEXC_FIN_MSG_SOCK);

	vl_MsgSock.len = (int)strlen(vl_MsgSock.msg);

	/*B emission reponse vers la tache TEzzz client */
	/*B SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */

	XZST_03EcritureTrace(XZSTC_MESSAGE, "Emission message <%s> \n lg:%d.", vl_MsgSock.msg, vl_MsgSock.len);

	sem_wait(&vg_semaphore);
	if (write(pa_CmdeLCR->sock_clnt, vl_MsgSock.msg, vl_MsgSock.len) != vl_MsgSock.len)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Emission message <%s> vers client TEzzz impossible.",
							 vl_MsgSock.msg);
	}
	sem_post(&vg_semaphore);
	/*B FIN SI */

	/*B O.K, retour XDC_OK */
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Traitement d'un message recu sur une sock. comms. avec un client TEzzz.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_traitement_msg_donn( XZEXT_Msg_Socket *pa_MsgSock )
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock	: pointeur sur le message recu sur la socket donnees active.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR :
*
*   NEANT.
*
* CONDITION D'UTILISATION
*
* pa_MsgSock doit contenir un message de la part d'un client TEzzz.
*
* FONCTION
* - si demande arret appel ecfg_sortir().
* - si demande init appel ecom_init().
* - si demande LCR :
*  	depouillement du message recu du client TEzzz ds. une struct type XZEXT_Cmde_LCR.
*  	empilement de cette meme demande ds. la liste des demandes a traiter.
*
------------------------------------------------------*/
void epro_traitement_msg_donn(XZEXT_Msg_Socket *pa_MsgSock)
{
	static char *version = "$Id: elmi_pro.c,v 1.25 2011/02/25 10:26:05 gesconf Exp $      : epro_traitement_msg_donn";

	XDY_Texte vl_TypeMsg;	   /* juste pour recuperer le type de message */
	XZEXT_Cmde_LCR vl_CmdeLCR; /* cmde LCR a empiler ds. la liste */
	XDY_Texte vl_Texte;
	int vl_JourSem; /* jour de la semaine */
	int vl_ValRet;

	/*A SI demande d'arret ALORS appel ecfg_sortir() */
	if (strstr(pa_MsgSock->msg, XZEXC_DEMANDE_ARRET) != NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_donn : demande  arret TELMi %d",
							 vg_Config.no_ligne);
		ecfg_sortir();
	}
	/*A SINON SI demande init. ALORS appel ecom_init() */
	else
	{
		if (strstr(pa_MsgSock->msg, XZEXC_DEMANDE_INIT) != NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_donn : demande  reinitialisation TELMi %d",
								 vg_Config.no_ligne);

			/*A SI init echoue pour cause de pb. ST ET alarme non envoyee */
			if ((vg_EtatST = ecom_init(&vg_Config, &vg_SockList, &vg_DesSocks)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "epro_traitement_msg_donn : Echec relance initialisation TELMi %d",
									 vg_Config.no_ligne);
			}
		}
		/*A SINON SI demande envoi trame LCR sur ligne serie geree ALORS */
		else
		{
			if (strstr(pa_MsgSock->msg, XZEXC_DEMANDE_LCR) != NULL)
			{
				/*B SI extraction cmde. LCR ecom_extraire_cmd_LCR() OK */
				if (ecom_extraire_cmd_LCR(pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr) == XDC_OK)
				{
					/*B ALORS insertion cmde LCR en liste d'attente ecom_insert_cmd_LCR() */
					ecom_insert_cmd_LCR(&vl_CmdeLCR, &pg_ListeCmdesLCR);
				}
			}
		}
	}
}

/*  Fichier : erau_cyc.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module permetant les declenchements cycliques
* de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	06 Dec 1994	: Creation
* Mismer.D	version 1.2	16 Jan 1995	:
* Nagiel.E	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	22 Mai 1995 	: Reprise entete, historique
* Mismer.D	version 1.5	09 Nov 1995	: Gestion etat de service equipement
* Diaz.L	version 1.6	09 Oct 1998     : Modif du au PC Niv.2
* JMg		version 1.7	21/11/07	: RAU au CI DEM/716
* JMg		version 1.8	22/11/07	: RAU au CI DEM/716
* JMG		version 1.9	13/12/07	: Suppression des reveil sur RAU_BASCULEMENT DEM716
* PNI		version 1.10	21/01/2008	: Sectorisation Aubagne-St Max -> Provence : Maj vl_pirau DEM743
* JMg		version 1.11	02/10/09	: secto DEM887
* JPL		version 1.13	01/12/11	: suppression warnings mineurs
* JMG		version 1.14	01/07/16	: emissio ncyclique basculement troncon
*  JMG           03/03/17 : regio DEM1220
* JPL		03/01/23 : Suppression commande periodique "ACT XT"  (SAE-447)
* GGY		07/09/23	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "erau_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "erau_cyc.c 1.16" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern T_Liste_Zones *pg_Zones;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique ( int va_NumSock )
/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	int					vl_IndCnx = 0,
						vl_LgMsg  = 0;
	XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
	XDY_Horodate		vl_Horodate;
	XZSMT_Horodate		vl_Horodatedecompo;
	int					vl_Delai;
	XZEXT_CLE_PROG		pl_CleProg;
	XZEXT_Trame			vl_tlcr;
	INFO_EQT_FEN_LCR	*pl_ListeEqt;
	int					vl_noeqt, i;
	char				vl_Cas = 0;
	XDY_Rgs       		vl_rgs_transfert = "";
	char				vl_trame[20];
	int					vl_pirau;
	T_Liste_Zones 		*pl_Zones = NULL;
	XDY_District 		vl_site;

	/* Lecture du message recu dans la socket appelante */
	vl_site = XZSC_22NumeroSiteLocal();
	if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
	{
		/* Ecriture trace Erreur lecture message socket. */
		XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
	}   
	else
	{
		/* Si le message est correct et si ce n'est pas une deconnexion */
		if ( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
		{
			/*A Si il s'agit d'un reveil cyclique */
			if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
			{
				sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
										&vl_Horodate,
										&vl_Delai,
										pl_CleProg );
				/* Demande periodique de la fenetre LCR */
				if (strstr(pl_CleProg,"RAU_FLcr_NoEqt:")!=NULL)
				{ 
					sscanf(pl_CleProg,"RAU_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
					ex_relance_lcr  ( 	vl_noeqt,
										vl_Cas,
										&pg_DebutListeRAUFLcr,
										vl_Horodate, 
										pl_CleProg,
										va_NumSock );
					return ( XDC_OK );
				}

				switch ( vl_Delai )
				{
					/*A Si il s'agit d'un reveil cyclique 1Mn */
					case ERAUC_REVEIL_1MN :
					{
										
						/*envoi commande basculement troncon*/
						if (!(vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV) ) 
						{
							pl_Zones = pg_Zones;
							while (pl_Zones!=NULL) 
							{
								if (pl_Zones->zone.site = vl_site) 
								{
									if (pl_Zones->zone.compteur==0) 
									{
										sprintf(vl_trame,"ACT XT MODE=%s ZONE=%s",
													pl_Zones->zone.etat,
													pl_Zones->zone.zone);				
																			
										if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
											EnvoyerTrame(	vl_trame,
															XZEXC_TRAME_RAU_TRONCON,
															vg_donsysrau.Config.AdresseRGS,
															XZEXC_NOCMD,
															XZEXC_PRIORITEFAIBLE,
															vg_donsysrau.SocketMaitre);
										else
											EnvoyerTrameIP(	vl_trame,
															XZEXC_TRAME_RAU_TRONCON,
															vg_donsysrau.Config.AdresseIpMaitre,
															vg_donsysrau.Config.PortIpMaitre,
															vg_donsysrau.Config.AdresseRGS,
															XZEXC_NOCMD,
															XZEXC_PRIORITEFAIBLE,
															vg_donsysrau.SocketMaitre);
									}
									else  
									{
										if (pl_Zones->zone.compteur>0)
											pl_Zones->zone.compteur--;
										XZST_03EcritureTrace ( XZSTC_INFO, "commande bascul en cours");
									}
									pl_Zones = pl_Zones->suivant;
								}
							}
						}				
						/* SAE-447  Envoi d'une commande "ACT XT PIR=n" supprime */				
						if ((vg_donsysrau.PC2 ==XDC_FAUX) && ( !(vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV) ))
						{
							if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
								EnvoyerTrame(	"ACT XL\0",
												XZEXC_TRAME_STAT_COMP,
												vg_donsysrau.Config.AdresseRGS,
												XZEXC_NOCMD,
												XZEXC_PRIORITEFAIBLE,
												vg_donsysrau.SocketMaitre);
							else
								EnvoyerTrameIP(	"ACT XL\0",
												XZEXC_TRAME_STAT_COMP,
												vg_donsysrau.Config.AdresseIpMaitre,
												vg_donsysrau.Config.PortIpMaitre,
												vg_donsysrau.Config.AdresseRGS,
												XZEXC_NOCMD,
												XZEXC_PRIORITEFAIBLE,
												vg_donsysrau.SocketMaitre);
						}		
					}
				}
			}
		
			/*A Si il s'agit d'un reveil pour synchro de la date */
			if ( strstr ( pl_CleProg, "RAU_Timer_Synchro" ) != NULL )
			{
				/*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
				XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
					
				/* passage de l'annee sur 2 digit */
				memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
					
				/*B Composition de la trame mise a la date et a l'heure */
				sprintf ( vl_tlcr, "DT %s",vl_Horodatedecompo);
				XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
				
				/*B Envoi trame  date et heure au rau*/
				if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
				{
					if ( EnvoyerTrame(	vl_tlcr,
										XZEXC_TRAME_DATE,
										vg_donsysrau.Config.AdresseRGS,
										XZEXC_NOCMD,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) == XDC_NOK )
						/* Rearmement du reveil synchro date */
						ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "RAU_Timer_Synchro", va_NumSock, pl_MsgTimer )
				}
				else
				{
					if ( EnvoyerTrameIP(	vl_trame,
											XZEXC_TRAME_DATE,
											vg_donsysrau.Config.AdresseIpMaitre,
											vg_donsysrau.Config.PortIpMaitre,
											vg_donsysrau.Config.AdresseRGS,
											XZEXC_NOCMD,
											XZEXC_PRIORITEFAIBLE,
											vg_donsysrau.SocketMaitre) == XDC_NOK )
						/* Rearmement du reveil synchro date */
						ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "RAU_Timer_Synchro", va_NumSock, pl_MsgTimer )
				}				
			}
		}
	}
	return ( XDC_OK );
}

																								
/*Fichier :  $Id: erau_cmd.c,v 1.41 2021/06/08 15:03:10 pc2dpdy Exp $      Release : $Revision: 1.41 $        Date : $Date: 2021/06/08 15:03:10 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le fichier des callbacks de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	01 Dec 1994	: Creation
* Nagiel.E	version 1.2	26 Dec 1994	:
* Mismer.D	version 1.3	16 Jan 1995	:
* Nagiel.E	version 1.4	24 Jan 1995	:
* Mismer.D	version 1.5	01 Fev 1995	:
* Mismer.D	version 1.6	11 Avr 1995	:
* Fontaine.C	version 1.7	22 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.8	01 Jul 1995	: Modification pour interface GN
* Volcic.F	version 1.9	02 Oct 1995	: Suppression controle antenne occupee
* Mismer.D	version 1.10	06 Nov 1995	: Modif pour GN
* Mismer.D	version 1.11	19 Dec 1995	: Modif pour init IHM GN (DEM/639)
* Mismer.D	version 1.12	20 Mai 1996	: Modif pour interruption test semi_auto  (DEM/1114)
* Mismer.D	version 1.13	28 Oct 1996	: Modif pour Alerte PAU HS (DEM/1308)
* Mismer.D	version 1.14	04 Dec 1996	: Suite modif pour Alerte PAU HS (DEM/1308)
* Mismer.D	version 1.15	30 Jan 1998	: modif pour basculement RAU (DEM/1538)
* Diaz.L	version 1.16    09 Oct 1998     : Modif du au PC Niv.2
* JMG		version 1.18    22/11/07	: RAU au CI DEM/716
* JMG		version 1.19	13/12/07	: Suppression des reveil sur RAU_BASCULEMENT DEM716
* PNI		version 1.20	21/01/2008									: Sectorisation Aubagne-St Max -> Provence : Maj vl_pirau DEM743
* PNI		version 1.21	25/03/2008 	: Faire le test sur toutes les antennes mais pour les PC niv 2 DEM767
* PNI		version 1.22	23/09/2009	: Analyse_Dg_Status : prise en cpte du / en debut de dg DEM910
* JMH		version 1.23	02/10/09	: secto DEM887
*  JMG      03/03/17 : regio DEM1220
* JPL		05/06/18 : Migration Linux 64 bits (DEM 1274) : Arguments complets pour appel XZST_10ArchiverMsgSyst  1.27
* GR        09/07/2020 : EMS 1.28
* ABE		01/04/2021 : PAU SONORISATION SAE-DEM244 1.29
* ABE		07/06/2021 : Fix Decodage message SONO et Test trame SONO SAE-DEM244 1.33
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* PNI       14/01/2022	: fix envoi valeur absolue du numero de commande  SAE-365
* GGY		07/09/23	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
* LCL		27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "erau_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: erau_cmd.c,v 1.41 2021/06/08 15:03:10 pc2dpdy Exp $      : erau_cmd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  commande de l'eqt en fonction de la demande.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_Init_IHM_Mes_RAU_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelï¿½e lors de la rï¿½ception du message
*  XDM_EINIT_ABT
*
* FONCTION 
*  Suivant la commande -> envoi du msg XDM_ETAT_RAU
*
------------------------------------------------------*/
{
static char *version = "$Id: erau_cmd.c,v 1.41 2021/06/08 15:03:10 pc2dpdy Exp $      :er_Init_IHM_Mes_RAU_cb " ;
   

   #ifndef _TIBCO_EMS
   T_STR		va_NomDGDesti;
   T_INT4	va_TypeEqt;
   T_INT4	va_NumEqt;
   #else
   XDY_Datagroup   va_NomDGDesti;
   XDY_Entier	va_TypeEqt;
   XDY_Entier	va_NumEqt;
   #endif 

   int		vl_ant = 0;

   ERAU_DONNEES_PAU *	pl_listePAU;
   XDY_Datagroup	pl_NomDG;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Init_IHM_Mes_RAU_cb ");
		  
   /*A Recuperation des parametres du message XDM_EINIT_ABT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_STR, &va_NomDGDesti
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_EINIT_ABT non lu." );
      XZST_10ArchiverMsgSyst(0,"er_Init_IHM_Mes_RAU_cb : Message XDM_EINIT_ABT recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Init_IHM_Mes_RAU_cb ");
      return;
   }

   XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_EINIT_ABT lu va_NomDGDesti=%s,va_TypeEqt=%d,va_NumEqt=%d",va_NomDGDesti,va_TypeEqt,va_NumEqt);

   /*A Suivant le type d'eqt -> envoi du msg correspondant */
   switch(va_TypeEqt)
   {
      /*B Traitement du systeme rau */
      case XZECC_TYPE_EQT_RAU:
      {
         /*  Pour 1 ou tous les RAU envoi du message XDM_ETAT_RAU */
         for ( pl_listePAU = pg_debutPAU; pl_listePAU != NULL; pl_listePAU = pl_listePAU->Suivant )
         {
XZST_03EcritureTrace(XZSTC_DEBUG3, "cherche PAU:%d, trouve=%d etat=%d",va_NumEqt,pl_listePAU->Config.Numero,pl_listePAU->Etat_Service);
            if (((va_NumEqt==0) || (va_NumEqt==pl_listePAU->Config.Numero)) &&
                 !(pl_listePAU->Etat_Service & XDC_EQT_HORS_SRV))
            {
      		if ( (vg_donsysrau.PC2 == XDC_VRAI) &&
        	     (pl_listePAU->Config.CodeAntenne > vg_donsysrau.Config.NbQuart) )
           		vl_ant = 1;
   		else
      		   vl_ant = pl_listePAU->Config.CodeAntenne;
      		   XZST_03EcritureTrace(XZSTC_DEBUG1, "pl_listePAU->Config.CodeAntenne=%d,vg_donsysrau.Config.NbQuart=%d,vg_donsysrau.PC2=%d,vg_donsysrau.Basculement[vl_ant]=%d %d %d",pl_listePAU->Config.CodeAntenne ,vg_donsysrau.Config.NbQuart,vg_donsysrau.PC2,vg_donsysrau.Basculement[vl_ant],XDC_GN_DONNE_RAU,vl_ant );
     	        if ( XDC_VRAI ) /*(vg_donsysrau.PC2 != XDC_VRAI) ||
     	             (vg_donsysrau.Basculement[vl_ant] == XDC_GN_DONNE_RAU) )*/
     	        {
     	           if (!TipcSrvMsgWrite ( va_NomDGDesti,
      			              XDM_IdentMsg(XDM_ETAT_RAU),
                          	      XDM_FLG_SRVMSGWRITE,
                         	      T_IPC_FT_INT4,pl_listePAU->Config.Numero,
                                      T_IPC_FT_INT4,(vg_etatPO1==pl_listePAU->Config.Numero)?1:(vg_etatPO2==pl_listePAU->Config.Numero)?2:(vg_etatGN==pl_listePAU->Config.Numero)?3:0,
                         	      T_IPC_FT_INT4,pl_listePAU->Etat.NoDerCmd,
                                      T_IPC_FT_INT4,pl_listePAU->Etat.CRCmd,
                        	      T_IPC_FT_INT4,(pl_listePAU->Etat.Gardepau == XDC_VRAI)?XZEUC_ETAT_GARDE:((vg_etatPO1==pl_listePAU->Config.Numero)||(vg_etatPO2==pl_listePAU->Config.Numero)||(vg_etatGN==pl_listePAU->Config.Numero))?XZEUC_ETAT_COMM:XZEUC_ETAT_REPOS,
                        	      T_IPC_FT_INT4,(vg_etatGN==XZEXC_EN_ECOUTE)?XZEUC_ETAT_APPEL:(vg_etatGN==XZEXC_LIBRE)?XZEUC_ETAT_REPOS:XZEUC_ETAT_COMM,
                          	      NULL))
      	          {
         	      XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_RAU non effectue." );
	          }
	           else
      	          {
         	      XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ETAT_RAU effectue sur %s %d" ,va_NomDGDesti,(pl_listePAU->Etat.Gardepau == XDC_VRAI)?XZEUC_ETAT_GARDE:((vg_etatPO1==pl_listePAU->Config.Numero)||(vg_etatPO2==pl_listePAU->Config.Numero)||(vg_etatGN==pl_listePAU->Config.Numero))?XZEUC_ETAT_COMM:XZEUC_ETAT_REPOS);
         	  }
	       }
	       else
	       {XZST_03EcritureTrace(XZSTC_WARNING, "pas dans la lsite => etat defaut");
     	           if (!TipcSrvMsgWrite ( va_NomDGDesti,
      			              XDM_IdentMsg(XDM_ETAT_RAU),
                          	      XDM_FLG_SRVMSGWRITE,
                         	      T_IPC_FT_INT4,pl_listePAU->Config.Numero,
                                      T_IPC_FT_INT4,0,
                         	      T_IPC_FT_INT4,pl_listePAU->Etat.NoDerCmd,
                                      T_IPC_FT_INT4,pl_listePAU->Etat.CRCmd,
                        	      T_IPC_FT_INT4,XZEUC_ETAT_DEFAUT,
                        	      T_IPC_FT_INT4,XZEUC_ETAT_DEFAUT,
                          	      NULL))
      	          {
         	      XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_RAU non effectue." );
	          }
	           else
      	          {
         	      XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_RAU effectue sur %s" ,va_NomDGDesti);
         	  }
	       }
            }
         }
         break;
      }       
   }
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Init_IHM_Mes_RAU_cb ");
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  commande de l'eqt en fonction de la demande.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_Cmd_RAU_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelï¿½e lors de la rï¿½ception du message
*  XDM_ECMD_RAU
*
* FONCTION 
*  Suivant la commande -> envoi de la commande correspondante
*
------------------------------------------------------*/
{
static char *version = "$Id: erau_cmd.c,v 1.41 2021/06/08 15:03:10 pc2dpdy Exp $      : er_Cmd_RAU_cb " ;

#ifdef _TIBCO_EMS
	XDY_Entier			vl_codecmd;
	XDY_Entier			vl_numPAU;
	XDY_Entier			vl_noPlat;
#else
	T_INT4			vl_codecmd;
	T_INT4			vl_numPAU;
	T_INT4			vl_noPlat;
#endif
   
	ERAU_DONNEES_PAU *		pl_ListePAU;
   
	XZEXT_Trame			vl_tlcr;
	int				vl_quarte;
	XDY_Rgs			vl_rgs_transfert = "";
	int				vl_pirau;
   
  	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Cmd_RAU_cb");
		  
   	if (!strcmp(vg_NomSite,XDC_NOM_SITE_CA)) vl_pirau=0;
   	else if (!strcmp(vg_NomSite,XDC_NOM_SITE_DP)) vl_pirau=2; /* DEM743*/
   	else if (!strcmp(vg_NomSite,XDC_NOM_SITE_VC)) vl_pirau=0;

   	/*A Recuperation des parametres du message XDM_ECMD_RAU */
   	if(!TipcMsgRead(pa_Data -> msg
 			  ,T_IPC_FT_INT4,&vl_codecmd
 			  ,T_IPC_FT_INT4,&vl_numPAU
 			  ,T_IPC_FT_INT4,&vl_noPlat
	       		  ,NULL))
   	{       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_RAU non lu" );
 		
       XZST_10ArchiverMsgSyst(0,"er_Cmd_RAU_cb : Message XDM_ECMD_RAU recu avec parametre(s) invalide(s)",NULL);
      
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_RAU_cb ");
      return;
   	}
    XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_RAU   vl_codecmd=%d,vl_numPAU=%d,vl_noPlat=%d",vl_codecmd,vl_numPAU,vl_noPlat);

   	if (vl_codecmd== XZEUC_ETAT_BASCUL) 
	{
    	XZIR02_Alerte_PAU_GN ( vg_donsysrau.Basculement[1], 1, 0 );
    	return;
   	}

   	/* Controle commande basculement */
   	if ( ((vl_codecmd == XZEUC_ECOUTE_GN) || (vl_codecmd == XZEUC_TRANS_GN)) && (vl_numPAU < 0) )
   	{
        /* Construction de la trame */
        strcpy ( vl_rgs_transfert,      vg_donsysrau.Config.AdresseRGS );
        /*sprintf(vl_tlcr, "ACT %cL %d\0", (vl_codecmd == XZEUC_ECOUTE_GN) ? '0' : 'X', abs(vl_numPAU) );*/
        sprintf(vl_tlcr, "ACT XT PIR=%d MODE=%s", vl_pirau,(vl_codecmd == XZEUC_ECOUTE_GN) ? "CI" : "LOC");
        /* Envoi de la trame */
        /*vl_rgs_transfert[1] = 'U';*/

		if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			EnvoyerTrame(	vl_tlcr,
							XZEXC_TRAME_TEST_COM,
							vl_rgs_transfert,
							(vl_codecmd == XZEUC_ECOUTE_GN) ? abs(vl_numPAU) : vl_numPAU,
		        			XZEXC_PRIORITEFAIBLE,
							vg_donsysrau.SocketMaitre);
		else
			EnvoyerTrameIP(	vl_tlcr,
							XZEXC_TRAME_TEST_COM,
							vg_donsysrau.Config.AdresseIpMaitre,
							vg_donsysrau.Config.PortIpMaitre,
							vl_rgs_transfert,
							(vl_codecmd == XZEUC_ECOUTE_GN) ? abs(vl_numPAU) : vl_numPAU,
		        			XZEXC_PRIORITEFAIBLE,
							vg_donsysrau.SocketMaitre);
		        
        XZST_03EcritureTrace(XZSTC_WARNING,"Passage en mode <%s>", vl_tlcr );
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_RAU_cb ");
        return;
   	} 
   
   	/*A Controle validite des elements du messge */
   	if (vl_codecmd != XZEUC_TST_SEMI_AUTO)
   	{
      	/* verif du no de PAU */
      	for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL ; pl_ListePAU = pl_ListePAU->Suivant )
      	{
         	if ( pl_ListePAU->Config.Numero == vl_numPAU ) break;
      	}
      	if ( pl_ListePAU == NULL )
      	{
        	XZST_03EcritureTrace(XZSTC_WARNING,"No de PAU Invalide %d", vl_numPAU);
        	/* message administrateur systeme */
        	XZST_10ArchiverMsgSyst(0,"er_Cmd_RAU_cb : Message XDM_ECMD_RAU recu avec parametre(s) invalides", NULL);
        	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_RAU_cb ");
        	return;
      	}   
   
      	/* Eqt hs ou inactif */
      	if ( pl_ListePAU->Etat_Service || vg_donsysrau.Etat_Service ) 
      	{
        	XZST_03EcritureTrace(XZSTC_WARNING,"RAU ou PAU HS ou INACTIF ou INHIBE" );
        	/* message administrateur systeme */
        	XZST_10ArchiverMsgSyst(0,"er_Cmd_RAU_cb : Message XDM_ECMD_RAU recu avec RAU ou PAU HS ou INACTIF ou INHIBE", NULL);
        	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_RAU_cb ");
        	return;
      	}
   	}
   
   	/*A Traitement de la commande */   
   	switch(vl_codecmd)
   	{
      	/*B Traitement de demande de communication avec un pau */
      	case XZEUC_DEM_COMM :
      	case XZEUC_APPEL :
      	{
			/* Construction de la trame */
			sprintf(vl_tlcr,"ACT XP %d %d %d"
							,pl_ListePAU->Config.CodeAntenne
							,pl_ListePAU->Config.CodePAU
							,vl_noPlat);

			/* Mise en memoire des donnees de la commande */
			pl_ListePAU->Etat.NoDerCmd=vl_codecmd;
				
			/* Envoi de la trame */
			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				if ( EnvoyerTrame (	vl_tlcr,
									XZEXC_TRAME_APPEL_PAU,
									vg_donsysrau.Config.AdresseRGS,
									vl_numPAU,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_OK)
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				}
				else 
				{
					if (vl_noPlat == 1) vg_etatPO1Fut = vl_numPAU;
					if (vl_noPlat == 2) vg_etatPO2Fut = vl_numPAU;
					pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}  
			else
			{
				if ( EnvoyerTrameIP(	vl_tlcr,
										XZEXC_TRAME_APPEL_PAU,
										vg_donsysrau.Config.AdresseIpMaitre,
										vg_donsysrau.Config.PortIpMaitre,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) != XDC_OK )
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				}
				else 
				{
					if (vl_noPlat == 1) vg_etatPO1Fut = vl_numPAU;
					if (vl_noPlat == 2) vg_etatPO2Fut = vl_numPAU;
					pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}
			break;
      	}
      	/*B Traitement de liberation ligne */
      	case XZEUC_LIB_COMM :
      	{
			/* Construction de la trame */
			sprintf(vl_tlcr,"ACT OP %d %d %d"
							,pl_ListePAU->Config.CodeAntenne
							,pl_ListePAU->Config.CodePAU
							,vl_noPlat);

			/* Mise en memoire des donnees de la commande */
			pl_ListePAU->Etat.NoDerCmd=vl_codecmd;
			
			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				/* Envoi de la trame */
				if (EnvoyerTrame (	vl_tlcr,
									XZEXC_TRAME_LIB_LIGNE,
									vg_donsysrau.Config.AdresseRGS,
									vl_numPAU,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre)!=XDC_OK)  
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				} 
				else
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}
			else
			{
				if ( EnvoyerTrameIP(	vl_tlcr,
										XZEXC_TRAME_LIB_LIGNE,
										vg_donsysrau.Config.AdresseIpMaitre,
										vg_donsysrau.Config.PortIpMaitre,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre)!=XDC_OK)  
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				} 
				else
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}
			break;
     	}
      	/*B Traitement de garde ligne */
      	case XZEUC_GARDE :
      	{
        	/* Construction de la trame */
        	sprintf(vl_tlcr,"ACT GP %d %d"
                        ,pl_ListePAU->Config.CodeAntenne
                        ,pl_ListePAU->Config.CodePAU);

        	/* Mise en memoire des donnees de la commande */
         	pl_ListePAU->Etat.NoDerCmd=vl_codecmd;
         	 
			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				/* Envoi de la trame */
				if (EnvoyerTrame (	vl_tlcr,
									XZEXC_TRAME_MISE_EN_GARDE,
									vg_donsysrau.Config.AdresseRGS,
									vl_numPAU,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_OK)
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				}
				else
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}
			else
			{
				if ( EnvoyerTrameIP (	vl_tlcr,
										XZEXC_TRAME_MISE_EN_GARDE,
										vg_donsysrau.Config.AdresseIpMaitre,
										vg_donsysrau.Config.PortIpMaitre,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) != XDC_OK)  
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				} 
				else
				{
					pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}
        	break;
      	}	
      	/*B Traitement de reprise ligne en garde */
      	case XZEUC_REPRISE :
      	{
        	/* Construction de la trame */
        	sprintf(vl_tlcr,"ACT RP %d %d %d"
                        ,pl_ListePAU->Config.CodeAntenne
                        ,pl_ListePAU->Config.CodePAU
                        ,vl_noPlat);
         
        	/* Mise en memoire des donnees de la commande */
        	pl_ListePAU->Etat.NoDerCmd=vl_codecmd;
         	
			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				/* Envoi de la trame */
				if ( EnvoyerTrame (	vl_tlcr,
									XZEXC_TRAME_REPRISE_GARDE,
									vg_donsysrau.Config.AdresseRGS,
									vl_numPAU,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_OK)
				{
						pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				}
				else
				{
						if (vl_noPlat == 1) vg_etatPO1Fut = vl_numPAU;
						if (vl_noPlat == 2) vg_etatPO2Fut = vl_numPAU;
						pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}
			else
			{
				if ( EnvoyerTrameIP (	vl_tlcr,
										XZEXC_TRAME_REPRISE_GARDE,
										vg_donsysrau.Config.AdresseIpMaitre,
										vg_donsysrau.Config.PortIpMaitre,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) != XDC_OK)  
				{
						pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				}
				else
				{
						if (vl_noPlat == 1) vg_etatPO1Fut = vl_numPAU;
						if (vl_noPlat == 2) vg_etatPO2Fut = vl_numPAU;
						pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}   
         	break;
		}
    	case XZEUC_TST_UNITAIRE :
      	{
        	/* Construction de la trame */
        	sprintf(vl_tlcr,"ACT TP %d %d %d"
                        ,pl_ListePAU->Config.CodeAntenne
                        ,pl_ListePAU->Config.CodePAU
                        ,vl_noPlat);

			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				/* Envoi de la trame */
				EnvoyerTrame (	vl_tlcr,
								XZEXC_TRAME_TST_UNITAIRE,
								vg_donsysrau.Config.AdresseRGS,
								vl_numPAU,
								XZEXC_PRIORITEFAIBLE,
								vg_donsysrau.SocketMaitre);
			}
			else
			{
				EnvoyerTrameIP (	vl_tlcr,
									XZEXC_TRAME_TST_UNITAIRE,
									vg_donsysrau.Config.AdresseIpMaitre,
									vg_donsysrau.Config.PortIpMaitre,
									vg_donsysrau.Config.AdresseRGS,
									vl_numPAU,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre);
			}
        	break;
      	}
      	case XZEUC_TST_SEMI_AUTO :
      	{
        	/* Construction de la trame */
			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				EnvoyerTrame ( "ACT TF",
								XZEXC_TRAME_TST_SEMI_AUTO,
								vg_donsysrau.Config.AdresseRGS,
								XZEXC_NOCMD,
								XZEXC_PRIORITEFAIBLE,
								vg_donsysrau.SocketMaitre );
		    }
			else
			{
				EnvoyerTrameIP (	"ACT TF",
									XZEXC_TRAME_TST_SEMI_AUTO,
									vg_donsysrau.Config.AdresseIpMaitre,
									vg_donsysrau.Config.PortIpMaitre,
									vg_donsysrau.Config.AdresseRGS,
									XZEXC_NOCMD,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre);
			}
/* DEM767 : Faire le test sur toutes les antennes mais pour les PC niv 2 */
/*         if ( vg_donsysrau.PC2 == XDC_VRAI )*/
/*         {*/
/*            if ( vg_donsysrau.Config.NbQuart > 1 )*/
/*               sprintf(vl_tlcr,"ACT TA %d %d", vl_numPAU, vl_noPlat);*/
/*            else*/
/*               sprintf(vl_tlcr,"ACT TT %d", vl_noPlat);*/
/*         }*/
/*         else*/

            sprintf(vl_tlcr,"ACT TT %d", vl_noPlat);

        	if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				/* Envoi de la trame */
				if (  EnvoyerTrame (	vl_tlcr,
										XZEXC_TRAME_TST_SEMI_AUTO,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) == XDC_OK ) 
				{	        
					vg_no_po_test =  vl_noPlat;
				}
            }
			else
			{
				if (  EnvoyerTrameIP (	vl_tlcr,
										XZEXC_TRAME_TST_SEMI_AUTO,
										vg_donsysrau.Config.AdresseIpMaitre,
										vg_donsysrau.Config.PortIpMaitre,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) == XDC_OK ) 
				{	        
					vg_no_po_test =  vl_noPlat;
				}
			}
         	break;
      	}
      	case XZEUC_TRANS_GN :
      	{         
        	/* Construction de la trame */
        	sprintf(vl_tlcr,"ACT XP %d %d 3"
                        ,pl_ListePAU->Config.CodeAntenne
                        ,pl_ListePAU->Config.CodePAU);

        	/* Mise en memoire des donnees de la commande */
        	pl_ListePAU->Etat.NoDerCmd=vl_codecmd;
         	 
        	/* Envoi de la trame */
        	strcpy ( vl_rgs_transfert, vg_donsysrau.Config.AdresseRGS );

			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				if( EnvoyerTrame (	vl_tlcr,
									XZEXC_TRAME_TRANS_GN,
									vg_donsysrau.Config.AdresseRGS,
									vl_numPAU,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_NOK)
				{
						pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				}
				else
				{
					vg_etatGNFut == vl_numPAU;
						pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
	        }
			else
			{
				if (  EnvoyerTrameIP (	vl_tlcr,
										XZEXC_TRAME_TRANS_GN,
										vg_donsysrau.Config.AdresseIpMaitre,
										vg_donsysrau.Config.PortIpMaitre,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) != XDC_NOK)
				{
						pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
				}
				else
				{
					vg_etatGNFut == vl_numPAU;
						pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
				}
			}
        	break;
      	}
      	case XZEUC_ECOUTE_GN :
      	{         
        	/* Construction de la trame */
        	if ( vg_donsysrau.PC2 == XDC_VRAI )
        		sprintf(vl_tlcr,"ACT B3 %d", pl_ListePAU->Config.CodeAntenne );
        	else
        		sprintf(vl_tlcr,"ACT B3");
         
        	/* Mise en memoire des donnees de la commande */
        	pl_ListePAU->Etat.NoDerCmd=vl_codecmd;
         	 
			/* Envoi de la trame */
        	if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
        		if ( EnvoyerTrame (	vl_tlcr,
									XZEXC_TRAME_ECOUTE_GN,
									vg_donsysrau.Config.AdresseRGS,
									vl_numPAU,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_NOK )
        		{
        		   pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
        		}
        		else
        		{
        		   pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
        		}
			}
			else
			{
				if (  EnvoyerTrameIP (	vl_tlcr,
										XZEXC_TRAME_ECOUTE_GN,
										vg_donsysrau.Config.AdresseIpMaitre,
										vg_donsysrau.Config.PortIpMaitre,
										vg_donsysrau.Config.AdresseRGS,
										vl_numPAU,
										XZEXC_PRIORITEFAIBLE,
										vg_donsysrau.SocketMaitre) != XDC_NOK)
				{
        		   pl_ListePAU->Etat.CRCmd=XZEXC_CRNOK;
        		}
        		else
        		{
        		   pl_ListePAU->Etat.CRCmd=XZEXC_CMD_EN_COURS;
        		}
			}
        	break;
      	}
   	}
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_RAU_cb ");
	       
}

void	er_Cmd_SONO_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelï¿½e lors de la rï¿½ception du message
*  XDM_ECMD_SONO
*
* FONCTION 
*  Suivant la commande -> envoi de la commande correspondante
*
------------------------------------------------------*/
{
static char *version = "$Id: erau_cmd.c,v 1.41 2021/06/08 15:03:10 pc2dpdy Exp $      : er_Cmd_SONO_cb " ;
   
	XZEXT_Trame				vl_tlcr;
	ERAU_DONNEES_RAU_SONO	vl_Commande_SONO;
	int 					vl_NoCMD;
	#ifndef _TIBCO_EMS
	T_STR					vl_NomMachine;
	T_STR 					vl_Message;
	T_STR 					vl_Autoroute;
	#else
	XDY_NomMachine			vl_NomMachine;
	XDY_MessageSONO			vl_Message;
	XDY_NomAuto				vl_Autoroute;
	#endif   

	XDY_Sens	vl_Sens;
	XDY_PR		vl_PR_debut;
	XDY_PR		vl_PR_fin;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Cmd_SONO_cb");
	/*A Recuperation des parametres du message XDM_ECMD_SONO */
	if(!TipcMsgRead(pa_Data -> msg   
			,T_IPC_FT_INT4, &vl_Sens
			,T_IPC_FT_INT4, &vl_PR_debut
			,T_IPC_FT_INT4, &vl_PR_fin
			,T_IPC_FT_INT4, &vl_NoCMD
			,T_IPC_FT_STR,  &vl_Message
			,T_IPC_FT_STR,  &vl_Autoroute
			,T_IPC_FT_STR,  &vl_NomMachine
	       		  ,NULL))
   	{       
    	/* Trace : Impossibilite de lire le message */
    	XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_SONO non lu" );
    	XZST_10ArchiverMsgSyst(0,"er_Cmd_SONO_cb : Message XDM_ECMD_SONO recu avec parametre(s) invalide(s)",NULL);
    	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_SONO_cb ");
    	return;
   	}

	XZST_03EcritureTrace(XZSTC_WARNING, "%s %s",vl_Message, vl_Autoroute);
	strcpy(vl_Commande_SONO.Message, (char*)vl_Message);
	strcpy(vl_Commande_SONO.Autoroute,(char*)vl_Autoroute);
	vl_Commande_SONO.Sens=vl_Sens;
	vl_Commande_SONO.PR_debut = vl_PR_debut;
	vl_Commande_SONO.PR_fin = vl_PR_fin;

    XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_SONO Message=%s\n Autoroute=%s\n Sens=%d\n PR_Debut=%d\n PR_Fin=%d\n NoCmd=%d\n NomMachine=%s",vl_Commande_SONO.Message,
																	vl_Commande_SONO.Autoroute,
																	vl_Commande_SONO.Sens,
																	vl_Commande_SONO.PR_debut,
																	vl_Commande_SONO.PR_fin,
																	vl_NoCMD,
																	vl_NomMachine);

	/*B Traitement de demande de communication avec un pau SONO*/
	if(!strcmp(vl_Commande_SONO.Message,XZEUC_SONO_ATTENTE) ||
		!strcmp(vl_Commande_SONO.Message,XZEUC_SONO_EVACUATION) ||
		!strcmp(vl_Commande_SONO.Message,XZEUC_SONO_REPOS))
	{
		/* Construction de la trame */
		sprintf(vl_tlcr,"ACT SONO=%s PRMIN=%d PRMAX=%d SENS=%d AUT=%s CMD=%d"
				,vl_Commande_SONO.Message
				,vl_Commande_SONO.PR_debut
				,vl_Commande_SONO.PR_fin
				,vl_Commande_SONO.Sens
		        ,vl_Commande_SONO.Autoroute
				,abs(vl_NoCMD));
		 	 
		/* Envoi de la trame */
		if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
		{
			if (EnvoyerTrame (	vl_tlcr,
								XZEXC_TRAME_CMD_RAU_SONO_STRING,
								vg_donsysrau.Config.AdresseRGS,
								vl_NoCMD,
								XZEXC_PRIORITEFAIBLE,
								vg_donsysrau.SocketMaitre) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "er_Cmd_SONO_cb : Commande en erreur.\nCmd:%s",vl_tlcr);
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "er_Cmd_SONO_cb : Commande en cours.\nCmd:%s",vl_tlcr);
			}
		}
		else
		{
			if (  EnvoyerTrameIP (	vl_tlcr,
									XZEXC_TRAME_CMD_RAU_SONO_STRING,
									vg_donsysrau.Config.AdresseIpMaitre,
									vg_donsysrau.Config.PortIpMaitre,
									vg_donsysrau.Config.AdresseRGS,
									vl_NoCMD,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "er_Cmd_SONO_cb : Commande en erreur.\nCmd:%s",vl_tlcr);
        	}
        	else
        	{
				XZST_03EcritureTrace(XZSTC_WARNING, "er_Cmd_SONO_cb : Commande en cours.\nCmd:%s",vl_tlcr);
        	}
		}
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "er_Cmd_SONO_cb : Message transmit invalide. Message:%s",vl_Commande_SONO.Message);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_SONO_cb ");
        return;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Cmd_SONO_cb ");       
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	er_CMD_LCR ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg)
		     
		     
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelï¿½e lors de la rï¿½ception du message
*  XDM_ELCR
*
* FONCTION 
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
static char *version = "$Id: erau_cmd.c,v 1.41 2021/06/08 15:03:10 pc2dpdy Exp $      : er_CMD_LCR";

#ifndef _TIBCO_EMS   
   T_INT2			vl_TypeEqt;
   T_INT2			vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2			vl_Periode;
   T_REAL8			vl_HrDebut;
   T_REAL8			vl_HrFin;
#else
   XDY_Mot			vl_TypeEqt;
   XDY_Mot			vl_NoEqt;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot			vl_Periode;
   XDY_Horodate			vl_HrDebut;
   XDY_Horodate			vl_HrFin;
#endif   

    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    #else
    XDY_NomMachine		vl_NomMachine;
    #endif   

   INFO_EQT_FEN_LCR	        pl_eqt,*pl_ListeEqt;
   XZEXT_MSG_SOCKET		pl_MsgTimer;
   XZEXT_CLE_PROG   		vl_clef;
   
   	/* Trace du message en entree de la fonction */
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN er_CMD_LCR");
	       
   	/*A Lecture du message */
   	if(!TipcMsgRead(data->msg,
   			T_IPC_FT_INT2,	&vl_TypeEqt,
   			T_IPC_FT_INT2,	&vl_NoEqt,
   			T_IPC_FT_STR,	&vl_FicCmdLcr,
   			T_IPC_FT_STR,	&vl_FicResLcr,
   			T_IPC_FT_INT2,	&vl_Periode,
   			T_IPC_FT_REAL8,&vl_HrDebut,
   			T_IPC_FT_REAL8,&vl_HrFin,
   			T_IPC_FT_STR,	&vl_NomMachine,
	          	NULL))
	{       
    	/* Trace : Impossibilite de lire le message */
    	XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ELCR non lu." );
    	XZST_10ArchiverMsgSyst(0,"er_CMD_LCR : Message XDM_ELCR(RAU) recu avec parametre(s) invalide(s)", NULL);
    	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_CMD_LCR");
    	return;
	}
	      
	/*A Construction de la liste globale des eqt a piloter par la fenetre LCR */
	pl_eqt.TypeEqt=vl_TypeEqt;
	strcpy(pl_eqt.NomFichCmdesLCR,vl_FicCmdLcr);
	strcpy(pl_eqt.NomFichResLCR,vl_FicResLcr);
	pl_eqt.Periode=vl_Periode;
	pl_eqt.HrDebut=vl_HrDebut;
	pl_eqt.HrFin=vl_HrFin;
	strcpy(pl_eqt.NomMachine,vl_NomMachine);
	pl_eqt.Position=0;
	pl_eqt.Suivant=NULL;
	pl_eqt.NoEqt=vl_NoEqt;
        
    /* Recherche si pas de commande LCR en cours sur equipement concerne */
    if ( (vl_TypeEqt == XDC_EQT_RAU) &&
         (ex_test_autorisation_eqt (	&pg_DebutListeRAUFLcr, 
    				 					&pl_eqt,
    				 					XDC_EQT_RAU) == XDC_NOK) )
    {
       return;
    }

	if (vl_TypeEqt == XDC_EQT_RAU)
	{
	   	if ( (vg_donsysrau.Config.Numero == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
	   	{
	 		if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{	    
				ex_init_lcr ( 	&pg_DebutListeRAUFLcr,
								pl_eqt,
								vg_donsysrau.SocketMaitre,
								vg_donsysrau.Config.AdresseRGS,
								vg_donsysrau.Config.Numero,
								"RAU_FLcr_NoEqt:",
								vg_SockTIM );
			}
			else
			{
				ex_init_lcr_ip ( 	&pg_DebutListeRAUFLcr,
									pl_eqt,
									vg_donsysrau.SocketMaitre,
									vg_donsysrau.Config.AdresseRGS,
									vg_donsysrau.Config.Numero,
									"RAU_FLcr_NoEqt:",
									vg_SockTIM,
									&vg_donsysrau.Config.AdresseIpMaitre,
									vg_donsysrau.Config.PortIpMaitre);
			}
		}
		else
	   	{
			XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
		}
	}
	else
	{
		XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
	}
	   
   	/* Fin du callback */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback er_CMD_LCR" );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contrï¿½le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelï¿½e lors de la rï¿½ception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
static char *version = "$Id: erau_cmd.c,v 1.41 2021/06/08 15:03:10 pc2dpdy Exp $      : er_controle_cb";

    #ifndef _TIBCO_EMS
   T_INT4			va_Etat;
    T_STR			va_NomMachine;
   T_INT4			va_TypeEqt;
   T_INT4			va_NumEqt;
    #else
   XDY_Entier			va_Etat;
    XDY_NomMachine		va_NomMachine;
   XDY_Entier			va_TypeEqt;
   XDY_Entier			va_NumEqt;
    #endif

   ERAU_DONNEES_PAU *		pl_listePAU;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_controle_cbe ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&va_Etat
                          ,T_IPC_FT_STR, &va_NomMachine
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu ." );
   }
	         
   switch ( va_TypeEqt )
   {
      /*A si l'eqt est un RAU */
      case XDC_EQT_RAU :
         if ( va_NumEqt == vg_donsysrau.Config.Numero )
         {
      	    if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
               vg_donsysrau.Etat_Service &= va_Etat;
            else
               vg_donsysrau.Etat_Service |= va_Etat;
            XZST_03EcritureTrace ( XZSTC_INFO, "Etat de service RAU :%d",vg_donsysrau.Etat_Service );
         }
         break;
         
      /*A si l'eqt est un PAU */
      case XDC_EQT_PAU :
         for ( pl_listePAU = pg_debutPAU; pl_listePAU != NULL; pl_listePAU = pl_listePAU->Suivant )
         {
            if ( va_NumEqt == pl_listePAU->Config.Numero )
            {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
                  pl_listePAU->Etat_Service &= va_Etat;
               else
                  pl_listePAU->Etat_Service |= va_Etat;
               
              XZST_03EcritureTrace ( XZSTC_INFO, "Etat de service :%d pour le PAU :%d (%d)", pl_listePAU->Etat_Service, pl_listePAU->Config.Numero, va_Etat ^ 0xFFFF );
	       break;
	    }
	 }
	 break;
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_controle_cb ");
}




/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* fonction utilisateur passe a
* XZAE103_Abt_Presence_Op_PCsimpl
*************************************************
* SEQUENCE D'APPEL :
*/

void er_Presence_PCS ( XZAET_EPresence_Op_PCsimpl      va_presence )
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
	char  				vl_variable[30];
	char  				vl_valeur[5];
	XZEXT_MSG_SOCKET	pl_MsgTimer;
	int 				i = 0;
	XZEXT_Trame			vl_tlcr;
	XDY_Rgs				vl_rgs_transfert = "";
	int					vl_pirau;

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN: er_presence_PCS");

   	if (!strcmp(vg_NomSite,XDC_NOM_SITE_CA)) vl_pirau=0;
   	else if (!strcmp(vg_NomSite,XDC_NOM_SITE_DP)) vl_pirau=2; /* DEM743*/
   	else if (!strcmp(vg_NomSite,XDC_NOM_SITE_VC)) vl_pirau=0;

	/*trace*/
	XZST_03EcritureTrace(XZSTC_INFO,"er_presence_PCS: presence %d, site %s",
					    va_presence.Presence,
					    va_presence.NomSite);

	/* Construction de l'adresse RGS */
	strcpy ( vl_rgs_transfert,      vg_donsysrau.Config.AdresseRGS );
	/*vl_rgs_transfert[1] = 'U';*/
	
	if ( !strcmp(va_presence.NomSite, vg_NomSite) )
	{
    	if ( va_presence.Presence == vg_donsysrau.Operateur )
        	return;
     	else
        	vg_donsysrau.Operateur = va_presence.Presence;
        
	if ( va_presence.Presence != XDC_NOK)
	{
		ETIM_ANNUL_REVEIL(vg_donsysrau.Config.TimeOutRetour,"ERAU_BASCUL_GN",vg_SockTIM,pl_MsgTimer)
		ETIM_REVEIL_CYCLIQUE(vg_donsysrau.Config.TimeOutRetour,"ERAU_BASCUL_GN",vg_SockTIM,pl_MsgTimer)

		sprintf(vl_tlcr, "ACT XT PIR=%d MODE=CI",vl_pirau);

		if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
		{
			EnvoyerTrame (	vl_tlcr,
							XZEXC_TRAME_TEST_COM,
							vg_donsysrau.Config.AdresseRGS,
							vl_pirau,
							XZEXC_PRIORITEFAIBLE,
							vg_donsysrau.SocketMaitre);
		}
		else
		{
			EnvoyerTrameIP (	vl_tlcr,
								XZEXC_TRAME_TEST_COM,
								vg_donsysrau.Config.AdresseIpMaitre,
								vg_donsysrau.Config.PortIpMaitre,
								vg_donsysrau.Config.AdresseRGS,
								vl_pirau,
								XZEXC_PRIORITEFAIBLE,
								vg_donsysrau.SocketMaitre);
		}
		XZST_03EcritureTrace(XZSTC_INFO,"er_presence_PCS: RAU donne au CI cmd=%s nocmd=%d",vl_tlcr,vl_pirau);
	}
	else
	{
		for ( i = 1; i <= vg_donsysrau.Config.NbQuart; i++ )
    	XZIR02_Alerte_PAU_GN ( vg_donsysrau.Basculement[i], i, 0 );

		sprintf(vl_tlcr, "ACT XT PIR=%d MODE=LOC",vl_pirau);

		if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
		{		
			EnvoyerTrame (	vl_tlcr,
							XZEXC_TRAME_TEST_COM,
							vg_donsysrau.Config.AdresseRGS,
							-vl_pirau,
							XZEXC_PRIORITEFAIBLE,
							vg_donsysrau.SocketMaitre);
		}
		else
		{
			EnvoyerTrameIP (	vl_tlcr,
								XZEXC_TRAME_TEST_COM,
								vg_donsysrau.Config.AdresseIpMaitre,
								vg_donsysrau.Config.PortIpMaitre,
								vg_donsysrau.Config.AdresseRGS,
								-vl_pirau,
								XZEXC_PRIORITEFAIBLE,
								vg_donsysrau.SocketMaitre);
		}
		XZST_03EcritureTrace(XZSTC_INFO,"er_presence_PCS: RAU en LOC cmd=%s nocmd=%d",vl_tlcr,-vl_pirau);

		for ( i = 1; i <= vg_donsysrau.Config.NbQuart; i++ )
		{
		     /* emission d'une demande de basculement */
/*     		if ( (vg_donsysrau.Basculement[i] == XDC_GN_PREND_RAU) 
			{
				XZIR02_Alerte_PAU_GN ( XDC_GN_DONNE_RAU, i, -1 );
				break;
			}
*/
		}   
		ETIM_ANNUL_REVEIL(vg_donsysrau.Config.TimeOutRetour,"ERAU_BASCUL_GN",vg_SockTIM,pl_MsgTimer)
		vg_donsysrau.Reveil = XDC_VRAI;
              
     }
  }
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT er_presence_PCS");

}

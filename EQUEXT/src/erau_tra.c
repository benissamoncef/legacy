/*E*/
/*Fichier :  $Id: erau_tra.c,v 1.53 2021/06/10 08:50:36 pc2dpdy Exp $        $Revision: 1.53 $      Release : $Revision: 1.53 $        Date : $Date: 2021/06/10 08:50:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant des eqt RAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	05 Dec 1994	: Creation
* Nagiel.E	version 1.2	26 Dec 1994	:
* Nagiel.E 	version 1.3	26 Dec 1994	:
* Mismer.D	version 1.4	16 Jan 1995	:
* Nagiel.E	version 1.5	24 Jan 1995	:
* Mismer.D	version 1.6	09 Fev 1995	:
* Mismer.D	version 1.7	11 Avr 1995	:
* Volcic.F	version 1.8	21 Avr 1995	:
* Volcic.F	version 1.9	24 Avr 1995	:
* Volcic.F	version 1.10	24 Avr 1995	:
* Volcic.F	version 1.11.33 Avr 1995	:
* Fontaine.C	version 1.12	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.13	16 Jun 1995	: Modif niveau trace
* Volcic.F	version 1.14	01 Aou 1995	: Remplacement envoi alerte GN par Message IHM textuelle 
* Mismer.D	version 1.15	03 Nov 1995	: Modif pour GN
* Mismer.D	version 1.16	30 Mai 1996	: Ajout appel fonction compteur de marche XZMA01 (DEM/643)
* Mismer.D	version 1.17	30 Oct 1996	: Mod								if pour Alerte PAU HS (DEM/1308)
* Mismer.D	version 1.18	12 Nov 1996	: Modif pour Alerte PAU HS Suite(DEM/1308)
* Diaz.L	version 1.19	09 Oct 1998	: Modif du au PC Niv.2
* JMG		version 1.23	21/11/07	: RAU au CI DEM/716
* PNI		version 1.33	21/01/2008	: Sectorisation Aubagne-St Max -> Provence : Maj vl_pirau DEM743
* LCL		version 1.25	03/12/2008	: ajout alarmes sur RAU DEM/850
* JMG		version 1.26	02/10/09	: secto DEM887
* JMG		version 1.27	01/03/11	: correction decodage trame pour 64 bits
* JMG		version 1.28	21/03/11	: traitement PAU virtuel 
* PNI		version 1.30	05/05/11	: Ajout fin d'alarme DEF_TRANS sur reception ERR=0 DEM983
* PNI		version 1.31	17/05/11	: suppressim traces WARNING
* JPL		version 1.32	01/12/11	: suppression warnings mineurs
* JPL		version 1.33	01/12/11	: Changement profil fonction d'envoi d'alerte localisee
* JMG		version 1.34	15/11/14	: linux
 *JMG		version 1.35	26/08/16	: de	codage trame etat de basculement troncon
*  JMG           03/03/17 : regio DEM1220	1.36
* JMG		07/05/19	: GAPLA DEM1334	1.37
* PNI		21/07/20	: GAPLA nouvelle alarme 23 + correctif dans eu_envoi_alarme_pau SAE-153	1.38
* PNI		22/03/21	: Correctif PAU Virtuel SAE-248 1.39
* ABE		04/04/21	: Ajout PAU SONO DEM-SAE244 1.41
* ABE		08/06/21	: Fix RAU SONO DEM-SAE244 1.53
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* PNI 		14/01/2022	: fix envoi valeur absolue du numero de commande SAE-365
* GGY		07/09/23	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
* LCL		27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzma.h"

#include "erau_tra.h"
#include "xzao910.h"
#include "xzao911.h"

/* definitions de constantes */
#define C_IPHONE        255


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static int    vm_nocmd=0;   
/* declaration de fonctions internes */
int eu_envoi_etat_pau (    ERAU_DONNEES_PAU *, int, int );
int eu_envoi_etat_test_pau ( int, int, int );
int eu_envoi_alarme_pau ( XDY_Horodate, ERAU_DONNEES_PAU *, XDY_TypeAlarme, XDY_Booleen );
int eu_envoi_acquittement ( XDY_Eqt );
int eu_envoi_etat_GN (    ERAU_DONNEES_PAU *, int );
int eu_traite_bascul_rau(char *, char *);
int eu_envoi_etat_rau_sono (ERAU_DONNEES_RAU_SONO, int);

/* definition de fonctions externes */
extern T_Liste_Zones *pg_Zones;


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame et traitement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket:socket de lecture
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
------------------------------------------------------*/
{
static char *version = "@(#)erau_tra.c	1.33	1.33 12/01/11      : em_traitement_trame" ;
   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame,*pl_trame2,*pl_trame3;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrrgs[10];
   int				vl_typetrame;
   char		                vl_Msg[256];
   int				vl_param;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   ERAU_DONNEES_PAU		*pl_ListePAU;
   ERAU_DONNEES_RAU_SONO vl_donnees_rau_sono;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd;   
   char                         vl_horod1[20],vl_date[20],
                                vl_horod2[9],vl_heure[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   int                          vl_NumERR = -1;
   int                          vl_trouve;
   int 				vl_typ_rep;
   char 			vl_arg[6];
   int     			vl_noant = 0, vl_nopau = 0, vl_numplat = 0;
   char   			vl_noplat[2],vl_etat[2];
   int     			vl_num_pau;
   int     			vl_result_test;
   int                          vl_FinTest;
   int                          vl_fin;
   int                          vl_po = 0;
   int				i = 0;
   int				vl_pirau; 
   char				vl_site_bascul[5];
   char				vl_trame[40];   
   char                         vl_antenne_pirau[15];
   char         vl_str_autoroute[10],vl_str_pr[10],vl_str_sens[10],vl_str_tel[50];
   char *vl_autoroute,*vl_pr,*vl_sens,vl_tel[50];
   XDY_Autoroute vl_int_autoroute;
   XDY_PR vl_int_pr;
   XDY_Sens vl_int_sens;
   char vl_zone[40];
   char vl_mode[5];
   int vl_update=0;
   T_Liste_Zones *pl_Zones = NULL;
   XDY_Datagroup vl_dg;

    /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }
   else
   {
      /*A Si deconnexion tache esclave  alors fin de la tache */
      if ( (vl_IndCnx == XZEXC_DECONNEC) && (va_socket == vg_donsysrau.SocketEsclave) )
      {
         /* Ecriture trace . */
         XZST_03EcritureTrace( XZSTC_WARNING, "Perte connexion avec la tache esclave alors : fin de la tache. " );
         ec_sortir();
      }
      
      XZST_03EcritureTrace( XZSTC_INFO, " --------------- message : %s", vl_Msg);
      /*A Scan du message provenant de la socket */
      vl_param=sscanf(vl_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
      					   	    vl_entete2,
      					   	    vl_horod1,
      					   	    vl_horod2,
      					   	    &vl_nbnonrep,
      					  	    &vl_nberrcrc,
      					  	    &vl_nocmd,
      					   	    vl_adrrgs,
      					  	    &vl_typetrame,
      					  	    &vl_typ_rep);
      					 
      if (vl_param != 10)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket impossible mes=%s",vl_Msg);
         return ( XDC_NOK );
      }
            					 
      /*A Conversion horodate */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);

      /*A Gestion des trames provenant de telei */

      if (vl_typetrame == XZEXC_TRAME_TELEI_KO)
      {
         /*B Recherche du No RAU concernï¿½ en cas de reponse telei */
         if ( vg_donsysrau.SocketEsclave != va_socket ) 
         {
            XZST_03EcritureTrace( XZSTC_WARNING, " Rau Non trouve en memoire mes=%s",vl_Msg);
            return ( XDC_NOK );
         }
         
         /* Envoi de l'alarme */
         sprintf(vl_arg,"%d",vl_nberrcrc);
         if ( !(vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV) )
             ex_env_alarme  ( vg_NomMachine,
		    vl_horodate,
		    XDC_EQT_RAU,
	            vg_donsysrau.Config.Numero,
	            XZAMC_ERR_CRC_TELEI,
	            XDC_VRAI,
	            vl_arg,
		    XZSC_22NumeroSiteLocal());
         XZST_03EcritureTrace( XZSTC_WARNING, " Err crc provenant du RAU");
         return ( XDC_NOK );
      }
      			
      if (vl_typetrame == XZEXC_TRAME_TELEI_OK)
      {
         /* Envoi fin de l'alarme */
         sprintf(vl_arg,"%d",vl_nberrcrc);
         if ( !(vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV) )
             ex_env_alarme  ( vg_NomMachine,
		    vl_horodate,
		    XDC_EQT_RAU,
	            vg_donsysrau.Config.Numero,
	            XZAMC_ERR_CRC_TELEI,
	            XDC_FAUX,
	            vl_arg ,
		    XZSC_22NumeroSiteLocal());
				
         /*B Recherche du No RAU concernï¿½ en cas de reponse telei */
         if ( vg_donsysrau.SocketEsclave != va_socket ) 
         {
            XZST_03EcritureTrace( XZSTC_WARNING, " Rau Non trouve en memoire mes=%s",vl_Msg);
            eu_envoi_acquittement ( XZEXC_TRAME_TELEI_NAK );
            return ( XDC_NOK );
         }
         else
         {
            strcpy(vl_adrrgs,vg_donsysrau.Config.AdresseRGS);
         }
         
         if (strstr(vl_Msg,"APAU") != NULL)
         {
            vl_typetrame = XZEXC_TRAME_ALERTE_RAU;
         }
	 else if (strstr(vl_Msg, "LIT") != NULL)
         {
            vl_typetrame = XZEXC_TRAME_ALERTE_GAPLA;
         }
         else
         {
            if (strstr(vl_Msg,"GEND") != NULL)
            {   
               vl_typetrame = XZEXC_TRAME_RAU_GEND;
            }
            else
            {
               if (strstr(vl_Msg,"ERR=") != NULL)
               {   
                  vl_typetrame = XZEXC_TRAME_RAU_TEST;
               }
               else
               {
                  if (strstr(vl_Msg,"PIRA") != NULL)
                  {   
                     /* Acquitement telei */   
                     eu_envoi_acquittement ( XZEXC_TRAME_TELEI_ACK );
                     return ( XDC_OK );
                  }
                  else
                  {
                     XZST_03EcritureTrace( XZSTC_WARNING, " Trame RAU incomprehensible mes=%s",vl_Msg);
                     /* Acquitement telei */   
                     eu_envoi_acquittement ( XZEXC_TRAME_TELEI_NAK );
                     return ( XDC_NOK );
                  }
               }   
            }
         }
      }
      else
      {
            /*A Traitement des alarmes transmission avec l'equipement */
            if ( ex_traite_alarme_com ( vg_NomMachine,
					XDC_EQT_RAU,
					vg_donsysrau.Config.Numero,
      					vl_typ_rep,
      					vl_nbnonrep,
      					vl_nberrcrc,
      					vl_typetrame,
      					vg_Mode_Fonct,
					XZSC_22NumeroSiteLocal()) == XDC_NOK )
            {
               XZST_03EcritureTrace( XZSTC_WARNING, " Erreur trame non transmise ou non comprise sur l'eqt no %d"
                                            ,vg_donsysrau.Config.Numero);
            }
            
            if ( (vl_typetrame != XZEXC_TRAME_FEN_LCR) && (vl_typetrame  != XZEXC_TRAME_TEST_COM) &&
	         (vl_typetrame != XZEXC_TRAME_RAU_TRONCON) && (vl_typetrame != XZEXC_TRAME_CMD_RAU_SONO_STRING) && 
		 (vl_typetrame != XZEXC_TRAME_ETAT_RAU_SONO_STRING) )
            {
               for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL ; pl_ListePAU = pl_ListePAU->Suivant )
                  if ( pl_ListePAU->Config.Numero == vl_nocmd ) 
                  {
                     pl_ListePAU->Etat.CRCmd = ((vl_typ_rep == XZEXC_REP_NAK) || (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)) ? XZEXC_CRNOK : XZEXC_CROK;
                     break;
                  }

               if ( (pl_ListePAU == NULL) && (vl_typetrame != XZEXC_TRAME_TST_SEMI_AUTO) )
               {
                  if ( vl_nocmd != 0 )
                  {
                     XZST_03EcritureTrace( XZSTC_WARNING, "Erreur numero.:%d de borne non trouvee en config", vl_nopau );
                     eu_envoi_acquittement ( XZEXC_TRAME_TELEI_ACK );
                  }
                  return ( XDC_NOK );
               }
               else
               {
                  if ( pl_ListePAU->Etat.CRCmd == XZEXC_CRNOK )
                  {
                     if ( vl_typetrame != XZEXC_TRAME_TST_SEMI_AUTO )
                     {
                        if ( vg_etatPO1 == vl_nocmd ) vl_po = 1;
                        if ( vg_etatPO2 == vl_nocmd ) vl_po = 2;
                        if ( vg_etatGN  == vl_nocmd ) vl_po = 3;
    	                eu_envoi_etat_pau( pl_ListePAU, 
    	             		           vl_po, 
    	             		           (pl_ListePAU->Etat.Gardepau == XDC_VRAI)?XZEUC_ETAT_GARDE:((vg_etatPO1==vl_nocmd)||(vg_etatPO2==vl_nocmd)||(vg_etatGN==vl_nocmd))?XZEUC_ETAT_COMM:XZEUC_ETAT_REPOS );
                     }
                     else
                     {
                        if ( !vg_donsysrau.Etat_Service )
    	                   eu_envoi_etat_test_pau(  -1, XZEXC_CRNOK, XZEUC_ETAT_REPOS );
    	             }
                     return ( XDC_OK );
                  }
               }
            }
      }
      
      /*B Recherche du No RAU concernï¿½ */
      if ( strcmp(vg_donsysrau.Config.AdresseRGS,vl_adrrgs) && 
           (vl_typetrame != XZEXC_TRAME_TEST_COM) &&
	   (vl_typetrame != XZEXC_TRAME_RAU_TRONCON) )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Rau Non trouve en memoire mes=%s",vl_Msg);
         return ( XDC_NOK );
      }
	 
      /*A On se positionne sur le debut de la trame */
      pl_trame=strstr(vl_Msg,XZEXC_SEP_TRAME)+4;

      /*A Recherche du type de trame */
      switch(vl_typetrame)
      {
         /*B C'est une trame de reponse pour la fenetre LCR */
         case XZEXC_TRAME_FEN_LCR:
         {
            /* Appel de ex_mrcr */
			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeRAUFLcr, vl_typ_rep );
			}
			else
			{
				ex_ret_LCR_IP( pl_trame, vl_adrrgs, &pg_DebutListeRAUFLcr, vl_typ_rep);
			}
            break;
         } 
         
	 /*trame de basculement d un troncon*/
	 case XZEXC_TRAME_RAU_TRONCON:
	       if ( (vl_typ_rep!=XZEXC_REP_NAK) && (vl_typ_rep!=XZEXC_TRAME_NON_TRANSM) &&
			 ((pl_trame2 = strstr ( pl_trame , "ACT XT ZONE=")) != NULL ) &&
			 ((pl_trame3 = strstr ( pl_trame , "MODE=")) !=NULL) )
	       {
		  vl_param = sscanf(pl_trame2 + 12, "%s",vl_zone);
		  vl_param = sscanf(pl_trame3 + 5, "%s", vl_mode);
		  XZST_03EcritureTrace( XZSTC_WARNING,"bascul %s = %s",vl_zone,vl_mode);

		  eu_traite_bascul_rau(vl_zone,vl_mode);
		}
            else
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Commande bascule <%s>", vl_Msg );
               if ((vl_typ_rep==XZEXC_REP_NAK) || (vl_typ_rep==XZEXC_TRAME_NON_TRANSM))
               {
                 XZST_03EcritureTrace( XZSTC_WARNING, " Impossible de transmettre la demande de basculement");
                 return ( XDC_NOK );
               }
            }
            break;

         /*B Trame de basculement */
         case XZEXC_TRAME_TEST_COM :
            if ( vl_nocmd == XZEXC_NOCMD )
            {
               XZST_03EcritureTrace( XZSTC_INFO, "Reponse bascule <%s>", vl_Msg );
               if ( (vl_typ_rep!=XZEXC_REP_NAK) && (vl_typ_rep!=XZEXC_TRAME_NON_TRANSM) &&
                    ((pl_trame2 = strstr ( pl_trame , "ACT XT PIR=")) != NULL ) &&
		    ((pl_trame3 = strstr ( pl_trame , "MODE=")) !=NULL) )
                    /*((pl_trame2 = strstr ( pl_trame , "ACT XL ")) != NULL ))*/
               {
                  vl_param = sscanf(pl_trame2 + 6,"%d",&vl_noant);
		  vl_param = sscanf(pl_trame3 + 5,"%2s",vl_site_bascul);
		  XZST_03EcritureTrace( XZSTC_DEBUG1,"%s / %s ", pl_trame3, vl_site_bascul);

                  for ( i = 1; i <= vg_donsysrau.Config.NbQuart; i ++ )
                  {
                     /*if ( vl_noant & (0x0001 << (i-1)) )*/
		     if (!strncmp(vl_site_bascul,"CI",2))
                     {
                        if ( vg_donsysrau.Basculement[i] != XDC_GN_PREND_RAU )
                        {
                            XZST_03EcritureTrace( XZSTC_WARNING, "Antenne %d sur CI", i );
                            XZIR02_Alerte_PAU_GN ( XDC_GN_PREND_RAU, i, 0 );
                            vg_donsysrau.Basculement[i] = XDC_GN_PREND_RAU;
                        }
                     }
                     else
                     {
                        if ( vg_donsysrau.Basculement[i] != XDC_GN_DONNE_RAU )
                        {
                            XZST_03EcritureTrace( XZSTC_WARNING, "Antenne %d sur district", i );
                            XZIR02_Alerte_PAU_GN ( XDC_GN_DONNE_RAU, i, 0 );
                            vg_donsysrau.Basculement[i] = XDC_GN_DONNE_RAU;
                        }
                     }
                  }
               }
	    if (vl_typ_rep==XZEXC_REP_ACK) 
		{
         if (!strcmp(vg_NomSite,XDC_NOM_SITE_CA)) vl_pirau=0;
         else if (!strcmp(vg_NomSite,XDC_NOM_SITE_DP)) vl_pirau=2; /* DEM743*/
         else if (!strcmp(vg_NomSite,XDC_NOM_SITE_VC)) vl_pirau=0;
			sprintf(vl_trame,"ACT XT PIR=%d\0",vl_pirau);

			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
				EnvoyerTrame(	vl_trame,
								XZEXC_TRAME_TEST_COM,
								vg_donsysrau.Config.AdresseRGS,
								XZEXC_NOCMD,
								XZEXC_PRIORITEFAIBLE,
								vg_donsysrau.SocketMaitre);
			else
				EnvoyerTrameIP(	vl_trame,
								XZEXC_TRAME_TEST_COM,
								vg_donsysrau.Config.AdresseIpMaitre,
								vg_donsysrau.Config.PortIpMaitre,
								vg_donsysrau.Config.AdresseRGS,
								XZEXC_NOCMD,
								XZEXC_PRIORITEFAIBLE,
								vg_donsysrau.SocketMaitre);
		}
        return ( XDC_OK );
    }
    else
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Commande bascule <%s>", vl_Msg );
               if ((vl_typ_rep==XZEXC_REP_NAK) || (vl_typ_rep==XZEXC_TRAME_NON_TRANSM))
               {
                 XZST_03EcritureTrace( XZSTC_WARNING, " Impossible de transmettre la demande de basculement");
                 return ( XDC_NOK );
               }
               if (vl_typ_rep==XZEXC_REP_ACK )
               {
                  XZIR02_Alerte_PAU_GN ( (vl_nocmd > 0) ? XDC_GN_PREND_RAU : XDC_GN_DONNE_RAU, abs(vl_nocmd), 0 );
                  vg_donsysrau.Basculement[1/*abs(vl_nocmd)*/] = (vl_nocmd > 0) ? XDC_GN_PREND_RAU : XDC_GN_DONNE_RAU;
                  XZST_03EcritureTrace( XZSTC_WARNING, "Succes demande de basculement %d ",vl_nocmd);
                 return ( XDC_OK );
               }
            }
            break;
         
         
	 /* GAPLA*/
	 case XZEXC_TRAME_ALERTE_GAPLA:
            pl_trame2 = strstr ( pl_trame , "LIT") + 4;
	    strcpy(vl_noplat,"O");
	    XZST_03EcritureTrace( XZSTC_WARNING, "TRAME %s",pl_trame2);
            vl_param=sscanf(pl_trame2,"%d %d %s",&vl_noant,
      					   	    &vl_nopau,
      					   	    vl_etat);
	    XZST_03EcritureTrace( XZSTC_WARNING, "(%d=) vl_noant %d vl_nopau %d vl_etat %s",
						vl_param, vl_noant,vl_nopau,vl_etat);
            for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL ; pl_ListePAU = pl_ListePAU->Suivant )
            {
               if ((pl_ListePAU->Config.CodePAU==vl_nopau) && ( pl_ListePAU->Config.CodeAntenne == vl_noant )) break;
            }
            if (pl_ListePAU != NULL) 
            {
               /* Acquitement telei */   
               eu_envoi_acquittement ( XZEXC_TRAME_TELEI_ACK );
            }
            else
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Erreur numero..:%d de borne non trouvee en configuration", vl_nopau );
               /* Acquitement telei */   
               eu_envoi_acquittement ( XZEXC_TRAME_TELEI_NAK );
               return ( XDC_NOK );
            }
            if ( vl_etat[0] == 'O' )
            {
               	     ex_env_alerte  (    vl_horodate,
                                   XDC_EQT_PAU,
				   pl_ListePAU->Config.Numero,
                                   XZAAC_LIT,
                                   vg_Mode_Fonct,
				   XZSC_22NumeroSiteLocal(),
                                   vl_noant,
                                   vl_nopau);
            }
               
	   break;

         /*B Changement d'etat des affectations */
         case XZEXC_TRAME_ALERTE_RAU:
            /* Scan du message */
            pl_trame2 = strstr ( pl_trame , "APAU") + 5;
	    strcpy(vl_noplat,"O");
	    XZST_03EcritureTrace( XZSTC_WARNING, "TRAME %s",pl_trame2);
            vl_param=sscanf(pl_trame2,"%d %d %s %s",&vl_noant,
      					   	    &vl_nopau,
      					   	    vl_noplat,
      					   	    vl_etat);
	    XZST_03EcritureTrace( XZSTC_WARNING, "(%d=) vl_noant %d vl_nopau %d vl_noplat %s vl_etat %s",
						vl_param, vl_noant,vl_nopau,vl_noplat,vl_etat);

	    /*correction pour passage 64 bits*/
#ifdef _HPUX_SOURCE 
	    if (vl_param!=4) {
	     vl_param=sscanf(pl_trame2,"%d %d %s",&vl_noant, 
						 &vl_nopau,
						     vl_noplat);
           	} 
#else
	    if (strcmp(vl_etat,"~|~")==0) {
	     vl_param=sscanf(pl_trame2,"%d %d %s",&vl_noant, 
						 &vl_nopau,
						     vl_noplat);
		XZST_03EcritureTrace( XZSTC_WARNING , "==>vl_noant %d vl_nopau %d vl_noplat %s",
					vl_noant,vl_nopau,vl_noplat);
	    }


#endif
		XZST_03EcritureTrace( XZSTC_WARNING , "vl_noant %d vl_nopau %d vl_noplat %s",
					vl_noant,vl_nopau,vl_noplat);
           	 
	    /*si IPHONE*/
	    if (vl_nopau==C_IPHONE) {
                /*recuperer aut, pr, sens, tel TC E APAU 1 255 O AUT=A8 PR=232345 SENS=1 TEL=0494599593*/
                vl_param=sscanf(pl_trame2,"%d %d %s %s %s %s %s",&vl_noant,
                                                    &vl_nopau,
                                                    vl_etat,
                                                    vl_str_autoroute,
                                                    vl_str_pr,
                                                    vl_str_sens,
                                                    vl_str_tel);
		XZST_03EcritureTrace( XZSTC_DEBUG1, "vl_str_autoroute %s vl_str_pr %s vl_str_sens %s vl_str_tel %s", vl_str_autoroute,vl_str_pr,vl_str_sens,vl_str_tel);
                vl_autoroute=vl_str_autoroute+4;
                vl_pr=vl_str_pr+3;
                vl_sens=vl_str_sens+5;
                vl_int_pr=atoi(vl_pr);
                vl_int_sens=atoi(vl_sens);
		strcpy(vl_noplat,vl_etat);
		XZST_03EcritureTrace( XZSTC_WARNING , "vl_noant %d vl_nopau %d vl_noplat %s",
					vl_noant,vl_nopau,vl_noplat);
            }

            for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL ; pl_ListePAU = pl_ListePAU->Suivant )
            {
               if ((pl_ListePAU->Config.CodePAU==vl_nopau) && ( pl_ListePAU->Config.CodeAntenne == vl_noant )) break;
            }
            
            if ( (pl_ListePAU != NULL) && ((vl_noplat[0] == 'O') || (vl_noplat[0] == 'F') ||  (vl_etat[0] == 'X') || (vl_etat[0] == 'F') ||  (vl_etat[0] == 'E') || (vl_etat[0] == 'O')) )
            {
               /* Acquitement telei */   
               eu_envoi_acquittement ( XZEXC_TRAME_TELEI_ACK );
            }
            else
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Erreur numero..:%d de borne non trouvee en configuration", vl_nopau );
               /* Acquitement telei */   
               eu_envoi_acquittement ( XZEXC_TRAME_TELEI_NAK );
               return ( XDC_NOK );
            }
            
            if ( (pl_ListePAU->Etat_Service & XDC_EQT_HORS_SRV) || (vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV) ) break;

            /*B Si c'est un appel du sae par une borne -> Envoi d'alerte */
            if ( vl_noplat[0] == 'O' )
            {
               /*si pas IPHONE*/
                if (vl_nopau!=C_IPHONE) {
               	     ex_env_alerte  (    vl_horodate,
                                   XDC_EQT_PAU,
				   pl_ListePAU->Config.Numero,
                                   XZAAC_APPEL_PAU,
                                   vg_Mode_Fonct,
				   XZSC_22NumeroSiteLocal(),
                                   vl_noant,
                                   vl_nopau);
               
                 /*B Envoi du message d'etat */
    	         eu_envoi_etat_pau( pl_ListePAU, 0, XZEUC_ETAT_APPEL );
  	       }
	       else {
                 ex_env_alerte_localisee  (    vl_horodate,
                                   XDC_EQT_PAU,
                                   pl_ListePAU->Config.Numero,
                                   XZAAC_APPEL_IPHONE,
                                   vg_Mode_Fonct,
                                   XZSC_22NumeroSiteLocal(),
                                   vl_autoroute,vl_int_pr,vl_int_sens,
                                   vl_str_tel);
	       }
            }
            else
            {
	       if (vl_noplat[0] == 'F') {
		 vl_numplat=1;
		 vl_etat[0] = vl_noplat[0];
	       }
	       else
                 vl_numplat = atoi ( vl_noplat );
               /* Si PC Niveau 2 et pas platine operateur alors prendre num platine - 3 (cad GN) */
               if ((vg_donsysrau.PC2 == XDC_VRAI) && (vl_numplat > 1 ) )
                   vl_numplat = 3;
                   
               switch ( vl_etat[0] )
               {
                  /*B Si c'est un Decrochï¿½ platine PC  */
                  case 'E' :
                     if (vl_nopau!=C_IPHONE) {
                       if (vl_numplat == 1) vg_etatPO1 = pl_ListePAU->Config.Numero;
                       if (vl_numplat == 2) vg_etatPO2 = pl_ListePAU->Config.Numero;
                       pl_ListePAU->Etat.Gardepau = XDC_FAUX;
  
                       /*B Envoi du message d'etat */
    	               if ( vl_numplat != 0 )
    	                  eu_envoi_etat_pau ( pl_ListePAU, vl_numplat, XZEUC_ETAT_COMM );
                       if ( vg_etatGN == XZEXC_LIBRE )
                          eu_envoi_etat_GN (	pl_ListePAU,
                        			  XDC_GN_FIN_ECOUTE_PAU );
		       }
    	             break;

                  /*B Si c'est un Racrochï¿½ platine PC  */
                  case 'F' :
		     if (vl_nopau!=C_IPHONE) {
                       if (vl_numplat == 1) vg_etatPO1 = XZEXC_LIBRE;
                       if (vl_numplat == 2) vg_etatPO2 = XZEXC_LIBRE;
                       if (vl_numplat == 3) vg_etatGN  = XZEXC_LIBRE;
                       pl_ListePAU->Etat.Gardepau = XDC_FAUX;
	       
                       /*B Envoi du message d'etat */
    	               if ( vl_numplat != 0 )
    	                  eu_envoi_etat_pau( pl_ListePAU, vl_numplat, XZEUC_ETAT_REPOS );
                       if ( vg_etatGN == XZEXC_LIBRE )
                          eu_envoi_etat_GN (	pl_ListePAU,
                        			XDC_GN_FIN_ECOUTE_PAU );
		     }
                     break;
		  case 'X':
		    sprintf(vl_tel,"NON ABOUTI %s",vl_str_tel);
		    ex_env_alerte_localisee  (    vl_horodate,
				XDC_EQT_PAU,
				 pl_ListePAU->Config.Numero,
				 XZAAC_APPEL_IPHONE,
				 vg_Mode_Fonct,
				 XZSC_22NumeroSiteLocal(),
				 vl_autoroute,vl_int_pr,vl_int_sens,
				 vl_tel);
		    break;

                  default :
                     XZST_03EcritureTrace( XZSTC_WARNING, " Trame etat APAU incomprehensible mes=%s",vl_Msg);
                     break;
               }
            }
            break;

         /*B Changement d'etat gendarme */
         case XZEXC_TRAME_RAU_GEND:
            /* Scan du message */
            pl_trame2 = strstr ( pl_trame , "GEND") + 5;
            vl_param = sscanf(pl_trame2,"%d %d %s",&vl_noant,&vl_nopau,vl_etat);
            
            for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL ; pl_ListePAU = pl_ListePAU->Suivant )
            {
               if ((pl_ListePAU->Config.CodePAU==vl_nopau) && ( pl_ListePAU->Config.CodeAntenne == vl_noant )) break;
            }
            
            if ( (pl_ListePAU != NULL) && (vl_param == 3) && ((vl_etat[0] == 'O') || (vl_etat[0] == 'F') ||  (vl_etat[0] == 'S') || (vl_etat[0] == 'R')) )
            {
               /* Acquitement telei */   
               eu_envoi_acquittement ( XZEXC_TRAME_TELEI_ACK );
            }
            else
            {
                /* Acquitement telei */   
                XZST_03EcritureTrace( XZSTC_WARNING, " Trame etat gendarme Pau incomprehensible mes=%s",vl_Msg);
                eu_envoi_acquittement ( XZEXC_TRAME_TELEI_NAK );
                return ( XDC_NOK );
            }
            
            if ( (pl_ListePAU->Etat_Service & XDC_EQT_HORS_SRV) || (vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV) ) break;
            
            switch ( vl_etat[0] )
            {
               /*B Si c'est un Debut GN en Ecoute -> mï¿½morisation info + alerte */
               case 'O' :
                  vg_etatGN  = pl_ListePAU->Config.Numero;
                  eu_envoi_etat_GN (		pl_ListePAU,
                  				XDC_GN_ECOUTE_PAU );
                  break;

               /*B Si c'est une Fin GN en Ecoute -> mï¿½morisation info + alerte */
               case 'F' :
                  vg_etatGN  = XZEXC_LIBRE;
                  eu_envoi_etat_GN (		pl_ListePAU,
                  				XDC_GN_FIN_ECOUTE_PAU );
                  break;

               /*B Si c'est un GN demande le secret -> alerte */
               case 'S' :
                  eu_envoi_etat_GN (		pl_ListePAU,
                  				XDC_GN_DEM_SECR_PAU );
                  break;

               /*B Si c'est un GN demande reprise SAE -> alerte */
               case 'R' :
                  eu_envoi_etat_GN (		pl_ListePAU,
                  				XDC_GN_FIN_ECOUTE_PAU );
                  vg_etatGN = XZEXC_LIBRE;
                  eu_envoi_etat_GN (		pl_ListePAU,
                  				XDC_GN_REPR_SAE_PAU );
                  pl_ListePAU->Etat.Gardepau = XDC_VRAI;
                  /*B Envoi du message d'etat */
    	          eu_envoi_etat_pau( pl_ListePAU, 3, XZEUC_ETAT_GARDE );
                  break;
            }  
            break;

         /*B Trame de test */
         case XZEXC_TRAME_RAU_TEST:
            /* scan du message */
            pl_trame2 = strstr ( pl_trame , "TC E") + 5;
            vl_param=sscanf(pl_trame2,"%s %s %d %d ERR=%d", vl_date, vl_heure, &vl_noant, &vl_nopau, &vl_NumERR );
	    
	    XZST_03EcritureTrace( XZSTC_INFO, "XZEXC_TRAME_RAU_TEST %d",vl_param);
	    if (vl_param != 5)
            {
               /* Si on n' a pas 5 parametres, il s'agit peut-etre d'une trame pour une antenne (4 parametres)*/
               vl_param=sscanf(pl_trame2,"%s %s %d ERR=%d", vl_date, vl_heure, &vl_noant, &vl_NumERR );
               if (vl_param != 4)
               {
                  XZST_03EcritureTrace( XZSTC_WARNING, " Trame d'erreur Pau incomprehensible mes=%s",vl_Msg);
                  /* Acquitement telei */
                  eu_envoi_acquittement ( XZEXC_TRAME_TELEI_NAK );
                  return ( XDC_NOK );
               }
               else
               {
		  XZST_03EcritureTrace( XZSTC_INFO, "XZEXC_TRAME_RAU_TEST2 : %d %d",vl_noant,vl_NumERR);
                  /* Acquitement telei */
                  eu_envoi_acquittement ( XZEXC_TRAME_TELEI_ACK );

                  /* recherche d'eventuelles erreurs  */
                  switch( vl_NumERR )
                  {
                    /*B formatter le message de numero d'antenne */
                    sprintf( vl_antenne_pirau,"ANTENNE %d", vl_noant );

                    /*B Si ERR=11 Alors envoi de l'alarme DEFAUT GENERAL ANTENNE */
                    case 11 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_GENERAL_ANTENNE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=12 Alors envoi de l'alarme REVEIL IMPOSSIBLE */
                    case 12 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_REVEIL_IMPOSSIBLE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=13 Alors envoi de l'alarme DEFAUT ALIMENTATION */
                    case 13 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ALIMENTATION,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=15 Alors envoi de l'alarme RESEAU MODE SUCURISE */
                    case 15 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_RESEAU_MODE_SECURISE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B A pertir d'ici, on traite une alarme sur le PIRAU (Antenne 0) */
                    /*B Si ERR=31 Alors envoi de l'alarme DEFAUT ALIMENTATION 5V FAIBLE */
                    case 31 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ALIM_5V_FAIBLE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=32 Alors envoi de l'alarme DEFAUT PARAMETRAGE */
                    case 32 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_PARAMETRAGE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=33 Alors envoi de l'alarme DEFAUT ABSENCE CARTES PO */
                    case 33 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_CARTES_PO,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=34 Alors envoi de l'alarme DEFAUT ABSENCE CARTES ANTENNES */
                    case 34 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_CARTES_ANTENN,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=35 Alors envoi de l'alarme DEFAUT TEMPERATURE TROP ELEVEE */
                    case 35 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_TEMP_TROP_ELEVEE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());

                    /*B Si ERR=36 Alors envoi de l'alarme DEFAUT ALIMENTATION 5V FORTE */
                    case 36 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ALIM_5V_FORTE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=37 Alors envoi de l'alarme DEFAUT RESET PAR BOUTON POUSSOIR */
                    case 37 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_RESET_BP,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=38 Alors envoi de l'alarme DEFAUT REBOOT PAR WDG */
                    case 38 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_REBOOT_WDG,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;


                    /*B Si ERR=41 Alors envoi de l'alarme DEFAUT ABSENCE PAU */
                    case 41 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_PAU,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=42 Alors envoi de l'alarme DEFAUT GENERAL CONFIGURATION */
                    case 42 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_GENERAL_CONFIG,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=43 Alors envoi de l'alarme DEFAUT ABSENCE PO LOGGE */
                    case 43 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_PO_LOGGE,
                                        XDC_VRAI,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       break;

                    /*B Si ERR=0 Alors envoi fin d'alarme pour l'antenne concernee */
                    case 0 :
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_GENERAL_ANTENNE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_REVEIL_IMPOSSIBLE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ALIMENTATION,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_RESEAU_MODE_SECURISE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ALIM_5V_FAIBLE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_PARAMETRAGE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_CARTES_PO,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_CARTES_ANTENN,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_TEMP_TROP_ELEVEE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ALIM_5V_FORTE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_RESET_BP,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_REBOOT_WDG,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_PAU,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_GENERAL_CONFIG,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                       ex_env_alarme  ( vg_NomMachine,
                                        vl_horodate,
                                        XDC_EQT_RAU,
                                        vg_donsysrau.Config.Numero,
                                        XZAMC_DEF_ABSENCE_PO_LOGGE,
                                        XDC_FAUX,
                                        vl_antenne_pirau,
                                        XZSC_22NumeroSiteLocal());
                 }
                 return ( XDC_OK );
              }
            }
            else
            {
               /* Acquitement telei */
               eu_envoi_acquittement ( XZEXC_TRAME_TELEI_ACK );
            }

            vl_FinTest = XDC_VRAI;
            vl_fin     = XDC_VRAI;
            vl_trouve  = XDC_FAUX;
            
		  XZST_03EcritureTrace( XZSTC_INFO, "XZEXC_TRAME_RAU_TEST3 : %d %d",vl_noant,vl_NumERR);

            for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL ; pl_ListePAU = pl_ListePAU->Suivant )
            {
                             
               if ((pl_ListePAU->Config.CodePAU==vl_nopau) && ( pl_ListePAU->Config.CodeAntenne == vl_noant))
               {
                  vl_trouve = XDC_VRAI;
                  vl_num_pau = pl_ListePAU->Config.Numero;
                  
                  pl_ListePAU->Etat.Test = XZEUC_ETAT_DEFAUT;
                  vl_result_test         = XZEUC_ETAT_DEFAUT;
                  
                  if ( !(pl_ListePAU->Etat_Service & XDC_EQT_HORS_SRV) && !(vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV) )
                  {
                     /* Recherche d'eventuel problemes */
                     switch ( vl_NumERR )
                     {
                         /*B Si ERR=1 Alors envoi de l'alarme PHONIE */
                         case 1 :
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PHONIE, XDC_VRAI );
                            break;
                         /*B Si ERR=101 Alors envoi fin de l'alarme PHONIE */
			 case 101 :
			    eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PHONIE, XDC_FAUX);
			    break;

                         /*B Si ERR=2 Alors envoi de l'alarme TRANS */
                         case 2 :
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_TRANS, XDC_VRAI );
                            break;
                         /*B Si ERR=102 Alors envoi fin de l'alarme TRANS */
			 case 102 :
			    eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_TRANS, XDC_FAUX);
			    break;

                         /*B Si ERR=3 Alors envoi de l'alarme PILE */
                         case 3 :
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PILE, XDC_VRAI );
                            break;
                         /*B Si ERR=103 Alors envoi fin de l'alarme PILE */
			 case 103:
			    eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PILE, XDC_FAUX);
			    break;

                         /*B Si ERR=4 Alors envoi de l'alarme DEFAUT ACCU */
			 case 4:
			  eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU,XZAMC_DEF_ACCU, XDC_VRAI);
			  break;
                         /*B Si ERR=104 Alors envoi fin de l'alarme DEFAUT ACCU */
			 case 104:
			   eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU,XZAMC_DEF_ACCU, XDC_FAUX);
			   break;

                         /*B Si ERR=7 Alors envoi de l'alarme DEFAUT BORNE HS */
			 case 7:
			   eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_BORNE_HS, XDC_VRAI);
			   break;
                         /*B Si ERR=107 Alors envoi fin de l'alarme DEFAUT BORNE HS */
			 case 107 :
			   eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_BORNE_HS, XDC_FAUX);
			   break;

                         /*B Si ERR=23 Alors envoi de l'alarme XZAMC_DEF_CAPT */
                         case 23 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_CAPT, XDC_VRAI );
                           break;
                         /*B Si ERR=1 23 Alors fin envoi de l'alarme XZAMC_DEF_CAPT */
                        case 123 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_CAPT, XDC_FAUX );
                           break;
                        /*B Si ERR=41 Alors envoi de l'alarme RECEP PRINC > 32 dBm */
                         case 41 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_PRIN_32_DBM, XDC_VRAI );
                           break;
                        /*B Si ERR=141 Alors envoi fin de l'alarme RECEP PRINC > 32 dBm */
                        case 141 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_PRIN_32_DBM, XDC_FAUX );
                           break;

                        /*B Si ERR=42 Alors envoi de l'alarme RECEP SECO > 32 dBm */
                        case 42 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_SECO_32_DBM, XDC_VRAI );
                           break;
                        /*B Si ERR=142 Alors envoi fin de l'alarme RECEP SECO > 32 dBm */
                        case 142 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_SECO_32_DBM, XDC_FAUX );
                           break;

                        /*B Si ERR=43 Alors envoi de l'alarme RECEP PRINC HORS SPECIF */
                        case 43 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_PRIN_HORS_SPEC, XDC_VRAI );
                           break;
                        /*B Si ERR=143 Alors envoi fin de l'alarme RECEP PRINC HORS SPECIF */
                        case 143 :
                           eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_PRIN_HORS_SPEC, XDC_FAUX );
                           break;

                         /*B Si ERR=44 Alors envoi de l'alarme RECEP SECO ACTIF*/
                        case 44 :
                          eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_SECO_ACTIF, XDC_VRAI );
                          break;
                         /*B Si ERR=144 Alors envoi fin de l'alarme RECEP SECO ACTIF*/
                        case 144 :
                          eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_SECO_ACTIF, XDC_FAUX );
                          break;

                        /*B Si ERR=45 Alors envoi de l'alarme EMETT SECO ACTIF*/
                        case 45 :
                          eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_EME_SECO_ACTIF, XDC_VRAI );
                          break;
                        /*B Si ERR=45 Alors envoi fin de l'alarme EMETT SECO ACTIF*/
                        case 145 :
                          eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_EME_SECO_ACTIF, XDC_FAUX );
                          break;

                        /*B Si ERR=46 Alors envoi de l'alarme PRESENCE EAU INTERFACE*/
                        case 46 :
                          eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PRESENCE_EAU_INTERF, XDC_VRAI );
                          break;
                        /*B Si ERR=146 Alors envoi fin de l'alarme PRESENCE EAU INTERFACE*/
			case 146 :
			  eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PRESENCE_EAU_INTERF, XDC_FAUX);
			  break;

                         /*B Si ERR=0 Alors envoi fin d'alarme pour le PAU concerne */
                         case 0 :
			    XZST_03EcritureTrace( XZSTC_INFO, "XZEXC_TRAME_RAU_TEST4  %d",vl_num_pau);
                            pl_ListePAU->Etat.Test = XZEUC_ETAT_REPOS;
                            vl_result_test         = XZEUC_ETAT_REPOS;
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PHONIE, 			XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PILE,   			XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_PRIN_32_DBM,       	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_SECO_32_DBM,       	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_PRIN_HORS_SPEC,    	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REC_SECO_ACTIF,        	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_EME_SECO_ACTIF,        	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PRESENCE_EAU_INTERF,   	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_GENERAL_ANTENNE, 	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REVEIL_IMPOSSIBLE,  	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ALIMENTATION,       	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_RESEAU_MODE_SECURISE,   	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ALIM_5V_FAIBLE,    	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_PARAMETRAGE,        	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ABSENCE_CARTES_PO,   	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ABSENCE_CARTES_ANTENN, 	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_TEMP_TROP_ELEVEE,   	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ALIM_5V_FORTE,       	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_RESET_BP,       		XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_REBOOT_WDG,    		XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ABSENCE_PAU,        	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_GENERAL_CONFIG,        	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ABSENCE_PO_LOGGE,   	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_BORNE_HS,        	XDC_FAUX );
                            eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_ACCU,        		XDC_FAUX );
			    eu_envoi_alarme_pau  ( vl_horodate, pl_ListePAU, XZAMC_DEF_TRANS, XDC_FAUX);                            
		break;
                        
                         default :
                            XZST_03EcritureTrace( XZSTC_WARNING, " Etat Erreur Pau incorrect mes=%s",vl_Msg);
                            return ( XDC_NOK );
                     }
                  }
               }
               
               if ((vl_fin == XDC_VRAI) && ( pl_ListePAU->Etat.Test == XZEUC_ETAT_EN_TEST)) 
               {
                  vl_FinTest = XDC_FAUX;
                  vl_fin     = XDC_FAUX;
               }
            }

            if ( vl_trouve != XDC_VRAI )
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Erreur numero :%d de borne non trouvee en config (ant=%d):%s", vl_nopau,vl_noant,vl_Msg );
               return ( XDC_NOK );
            }

            XZST_03EcritureTrace( XZSTC_INFO, "reception test pau:%d ant:%d:%s",vl_nopau,vl_noant ,vl_Msg);

            if ((vl_trouve == XDC_VRAI) && (vg_test_auto_en_cours == XDC_VRAI))
            {
               /*B Envoi du message d'etat */
               if ( !pl_ListePAU->Etat_Service && !vg_donsysrau.Etat_Service )
    	          eu_envoi_etat_test_pau ( vl_num_pau, (vl_FinTest==XDC_VRAI)?XZEXC_FIN_TEST:XZEXC_CMD_EN_COURS, vl_result_test );
 	       if (vl_FinTest==XDC_VRAI) vg_no_po_test = 0;
	    }
	    if (vl_FinTest==XDC_VRAI) vg_test_auto_en_cours = XDC_FAUX;
            
            break;

         /*B Acquitement Commandes Liberations */
         case XZEXC_TRAME_LIB_LIGNE:
         case XZEXC_TRAME_MISE_EN_GARDE:
            if ( pl_ListePAU->Etat.CRCmd == XZEXC_CROK )
            {
               if ( vl_typetrame == XZEXC_TRAME_MISE_EN_GARDE )
                  pl_ListePAU->Etat.Gardepau = XDC_VRAI;

               if ( vg_etatPO1 == vl_nocmd ) { vl_po = 1; vg_etatPO1 = XZEXC_LIBRE; }
               if ( vg_etatPO2 == vl_nocmd ) { vl_po = 2; vg_etatPO2 = XZEXC_LIBRE; }

               /*B Envoi du message d'etat */
    	       eu_envoi_etat_pau( pl_ListePAU, 
    	          		  vl_po, 
    	          	          (pl_ListePAU->Etat.Gardepau == XDC_VRAI)?XZEUC_ETAT_GARDE : XZEUC_ETAT_REPOS );
	    }
            break;

         /*B Acquitement Commandes Communication */
         case XZEXC_TRAME_REPRISE_GARDE:
         case XZEXC_TRAME_APPEL_PAU:
            pl_ListePAU->Etat.Gardepau = (pl_ListePAU->Etat.CRCmd == XZEXC_CROK) ? XDC_FAUX : pl_ListePAU->Etat.Gardepau;

            if ( vl_nocmd == vg_etatPO1Fut ) { vl_po = 1; vg_etatPO1 = vg_etatPO1Fut; }
            if ( vl_nocmd == vg_etatPO2Fut ) { vl_po = 2; vg_etatPO2 = vg_etatPO2Fut; }

            /*B Envoi du message d'etat */
    	    if ( vl_po !=  0 )
    	       eu_envoi_etat_pau( pl_ListePAU, vl_po, XZEUC_ETAT_COMM );

            break;

         /*B Acquitement Commandes TRANS GN*/
         case XZEXC_TRAME_TRANS_GN:
            pl_ListePAU->Etat.Gardepau = (pl_ListePAU->Etat.CRCmd == XZEXC_CROK) ? XDC_FAUX : pl_ListePAU->Etat.Gardepau;

            vg_etatGN  = vg_etatGNFut;
            if ( vg_etatPO1 == vl_nocmd ) { vl_po = 1; vg_etatPO1 = XZEXC_LIBRE; }
            if ( vg_etatPO2 == vl_nocmd ) { vl_po = 2; vg_etatPO2 = XZEXC_LIBRE; }

            /*B Envoi du message d'etat */
    	    if ( vl_po != 0 )
    	       eu_envoi_etat_pau( pl_ListePAU, vl_po, XZEUC_ETAT_REPOS );
            break;

         case XZEXC_TRAME_TST_SEMI_AUTO:
            for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL ; pl_ListePAU = pl_ListePAU->Suivant )
               pl_ListePAU->Etat.Test = XZEUC_ETAT_EN_TEST;
            vg_test_auto_en_cours = XDC_VRAI;
    	    eu_envoi_etat_test_pau(  -1, XZEXC_CROK, XZEUC_ETAT_REPOS );
            break;
	 case XZEXC_TRAME_CMD_RAU_SONO_STRING:
		if(vl_typ_rep==XZEXC_REP_ACK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO Action Acknowledge");
			
			/* Envoie trame demande ETAT RAU SONO */
			vm_nocmd=vl_nocmd;
			sprintf(vl_trame,"ACT SONO CMD=%d",abs(vl_nocmd));

			if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			{
				if (EnvoyerTrame (	vl_trame,
									XZEXC_TRAME_ETAT_RAU_SONO_STRING,
									vg_donsysrau.Config.AdresseRGS,
									vl_nocmd,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO Commande Etat en erreur.\nCmd:%s",vl_trame);
					xzat551_CompteRendu_RAU_SONO(vl_nocmd, vl_horodate, XDC_NOK, vg_NomSite);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO Commande en cours.\nCmd:%s",vl_trame);
				}
			}
			else
			{
				if (EnvoyerTrameIP(	vl_trame,
									XZEXC_TRAME_ETAT_RAU_SONO_STRING,
									vg_donsysrau.Config.AdresseIpMaitre,
									vg_donsysrau.Config.PortIpMaitre,
									vg_donsysrau.Config.AdresseRGS,
									vl_nocmd,
									XZEXC_PRIORITEFAIBLE,
									vg_donsysrau.SocketMaitre) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO Commande Etat en erreur.\nCmd:%s",vl_trame);
					xzat551_CompteRendu_RAU_SONO(vl_nocmd, vl_horodate, XDC_NOK, vg_NomSite);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO Commande en cours.\nCmd:%s",vl_trame);
				}
			}
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO Action Not Acknowledge");
		}
	 	break;
	 
	 case XZEXC_TRAME_ETAT_RAU_SONO_STRING:
	 
	//Valeur initiale des pau pilote erreur
	vl_donnees_rau_sono.Nb_PAUErreur =0;
	vl_donnees_rau_sono.Nb_PAUPilotes =0;
		XZST_03EcritureTrace(XZSTC_WARNING,"pl_trame:%s",pl_trame);
		if(sscanf(pl_trame,"SONO=%s ERR=%d NB=%d CMD=%d",vl_donnees_rau_sono.Message,
							&vl_donnees_rau_sono.Nb_PAUErreur,
							&vl_donnees_rau_sono.Nb_PAUPilotes,
							&vl_nocmd) < 4)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO ETAT : Erreur Retour ï¿½quipement");
			xzat551_CompteRendu_RAU_SONO(vm_nocmd, vl_horodate, XDC_NOK, vg_NomSite, vl_donnees_rau_sono.Nb_PAUErreur, vl_donnees_rau_sono.Nb_PAUPilotes);
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO ETAT : Cmd:%d, Message:%s, ERR=%d, PILOTE:%d",vl_nocmd,
								vl_donnees_rau_sono.Message,
								vl_donnees_rau_sono.Nb_PAUErreur,
								vl_donnees_rau_sono.Nb_PAUPilotes);
			if(vl_donnees_rau_sono.Nb_PAUErreur>0)
				xzat551_CompteRendu_RAU_SONO(vm_nocmd, vl_horodate, XDC_NOK, vg_NomSite, vl_donnees_rau_sono.Nb_PAUErreur, vl_donnees_rau_sono.Nb_PAUPilotes);
			else
				xzat551_CompteRendu_RAU_SONO(vm_nocmd, vl_horodate, XDC_OK, vg_NomSite, vl_donnees_rau_sono.Nb_PAUErreur, vl_donnees_rau_sono.Nb_PAUPilotes);
		
			eu_envoi_etat_rau_sono(vl_donnees_rau_sono, vm_nocmd);
		}
	 
	 break;
      }
   }

   return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de l'etat d'un PAU
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int eu_envoi_etat_pau (    ERAU_DONNEES_PAU		* pa_ListePAU,
			   int				va_Platine,
			   int				va_Etat )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
   static char *version = "@(#)erau_tra.c	1.33	1.33 12/01/11      : eu_envoi_etat_pau" ;
   XDY_Datagroup		pl_NomDG;
   XZSCT_NomTache        	vl_NomTache   = "";
   XDY_NomMachine       	vl_NomMachine = "";
   int				vl_ant = 0;


   if ( pa_ListePAU->Etat_Service || vg_donsysrau.Etat_Service ) return (XDC_NOK );

   /*B Envoi du message d'etat */
   XDG_EncodeDG2 ( pl_NomDG, XDG_EETAT_RAU, vg_NomSite) ;			
   if ( (vg_donsysrau.PC2 == XDC_VRAI) &&
        (pa_ListePAU->Config.CodeAntenne > vg_donsysrau.Config.NbQuart) )
           vl_ant = 1;
   else
       vl_ant = pa_ListePAU->Config.CodeAntenne;
      		   
   XZST_03EcritureTrace(XZSTC_WARNING, "eu_envoi_etat_pau: bascul = %d;vg_donsysrau.PC2=%d,va_Etat=%d",vg_donsysrau.Basculement[1],vg_donsysrau.PC2,va_Etat );

   if ( (vg_donsysrau.PC2 != XDC_VRAI) ||
        (vg_donsysrau.Basculement[1] == XDC_GN_DONNE_RAU) )
   {
      if (!TipcSrvMsgWrite ( pl_NomDG,
      			  XDM_IdentMsg(XDM_ETAT_RAU),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, pa_ListePAU->Config.Numero,
                          T_IPC_FT_INT4, va_Platine,
                          T_IPC_FT_INT4, pa_ListePAU->Etat.NoDerCmd,
                          T_IPC_FT_INT4, pa_ListePAU->Etat.CRCmd,
                          T_IPC_FT_INT4, va_Etat,
                          T_IPC_FT_INT4, (vg_etatGN==XZEXC_EN_ECOUTE)?XZEUC_ETAT_APPEL:(vg_etatGN==XZEXC_LIBRE)?XZEUC_ETAT_REPOS:XZEUC_ETAT_COMM,
                          NULL))
     {
        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_RAU non effectue." );
     }
     else
     {
        XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_RAU effectue sur %s" ,pl_NomDG);
     }
  }

  /* Mise jour compteur de marche equipement */
  XZSC_06NomTache( vl_NomTache );
  XZSC_07NomMachine( vl_NomMachine );
  if ( va_Etat == XZEUC_ETAT_REPOS )
  {
	   XZMA01_Maj_duree_marche (  pa_ListePAU->Config.Numero,
                                      XDC_EQT_PAU,
                                      XDC_FAUX,
                                      vl_NomTache,
                                      vl_NomMachine );
  }
  else
  {
	   XZMA01_Maj_duree_marche (  pa_ListePAU->Config.Numero,
                                      XDC_EQT_PAU,
                                      XDC_VRAI,
                                      vl_NomTache,
                                      vl_NomMachine );
  }

  return ( XDC_OK );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de l'etat avec GN
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int eu_envoi_etat_GN (    ERAU_DONNEES_PAU		* pa_ListePAU,
			  int				va_Etat )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
   static char *version = "@(#)erau_tra.c	1.33	1.33 12/01/11      : eu_envoi_etat_GN" ;
   int 	vl_ant = 0;
   
   if ( (vg_donsysrau.PC2 == XDC_VRAI) &&
        (pa_ListePAU->Config.CodeAntenne > vg_donsysrau.Config.NbQuart) )
           vl_ant = 1;
   else
      vl_ant = pa_ListePAU->Config.CodeAntenne;


   if ( (vg_donsysrau.Basculement[vl_ant] == XDC_GN_DONNE_RAU) ||
        (vg_donsysrau.PC2 != XDC_VRAI) )
   {
      XZIR02_Alerte_PAU_GN (	va_Etat, 
                        	pa_ListePAU->Config.CodeAntenne, 
                        	pa_ListePAU->Config.Numero );
   }

   return ( XDC_OK );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de l'etat d'un PAU en test
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int eu_envoi_etat_test_pau ( int	va_NumPAU,
			     int	va_Test,
			     int	va_ResultTest  )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
   static char *version = "@(#)erau_tra.c	1.33	1.33 12/01/11      : eu_envoi_etat_test_pau" ;
   XDY_Datagroup		pl_NomDG;



   /*B Envoi du message d'etat */
   XDG_EncodeDG2 ( pl_NomDG, XDG_EETAT_RAU, vg_NomSite) ;			
   if (!TipcSrvMsgWrite ( pl_NomDG,
      			  XDM_IdentMsg(XDM_ETAT_RAU),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_NumPAU,
                          T_IPC_FT_INT4, vg_no_po_test,
                          T_IPC_FT_INT4, XZEUC_TST_SEMI_AUTO,
                          T_IPC_FT_INT4, va_Test,
                          T_IPC_FT_INT4, va_ResultTest,
                          T_IPC_FT_INT4, (vg_etatGN==XZEXC_EN_ECOUTE)?XZEUC_ETAT_APPEL:(vg_etatGN==XZEXC_LIBRE)?XZEUC_ETAT_REPOS:XZEUC_ETAT_COMM,
                          NULL))
  {
      XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message (test PAU) XDM_ETAT_RAU non effectue." );
  }
  else
  {
      XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message (test PAU) XDM_ETAT_RAU effectue sur %s" ,pl_NomDG);
  }

   return ( XDC_OK );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de alarme d'un PAU
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int eu_envoi_alarme_pau ( XDY_Horodate		va_Horodate,
			  ERAU_DONNEES_PAU    * pa_ListePAU,
			  XDY_TypeAlarme	va_Type,
			  XDY_Booleen		va_Etat  )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
   static char *version = "@(#)erau_tra.c	1.33	1.33 12/01/11      : eu_envoi_alarme_pau " ;
   int vl_ant = 0;
  
   if ( (vg_donsysrau.PC2 == XDC_VRAI) &&
        (pa_ListePAU->Config.CodeAntenne > vg_donsysrau.Config.NbQuart) )
           vl_ant = 1;
   else
       vl_ant = pa_ListePAU->Config.CodeAntenne;

   XZST_03EcritureTrace( XZSTC_INFO,"eu_envoi_alarme_pau:PC2:%d,Basculement[vl_ant]:%d,va_Etat:%d\n type:%d;etat:%d;numero:%d;tt:%d;site:%d",vg_donsysrau.PC2,vg_donsysrau.Basculement[vl_ant],va_Etat,
	va_Type,va_Etat,pa_ListePAU->Config.Numero,(va_Type != XZAMC_ERR_CRC_TELEI) ? XDC_EQT_PAU :XDC_EQT_RAU,XZSC_22NumeroSiteLocal());
   if ( (vg_donsysrau.PC2 != XDC_VRAI) ||
        (vg_donsysrau.Basculement[vl_ant] == XDC_GN_DONNE_RAU) ||
        (vg_donsysrau.Basculement[vl_ant] == XDC_FAUX) ||
     	(va_Etat == XDC_FAUX) )
   {
	XZST_03EcritureTrace( XZSTC_INFO , "eu_envoi_alarme_pau:type:%d;etat:%d;numero:%d;tt:%d;site:%d",va_Type,va_Etat,pa_ListePAU->Config.Numero,(va_Type != XZAMC_ERR_CRC_TELEI) ? XDC_EQT_PAU : XDC_EQT_RAU,XZSC_22NumeroSiteLocal());

      ex_env_alarme  ( vg_NomMachine,
		    va_Horodate,
		    (va_Type != XZAMC_ERR_CRC_TELEI) ? XDC_EQT_PAU : XDC_EQT_RAU,
	            pa_ListePAU->Config.Numero,
	            va_Type,
	            va_Etat,
	            NULL,
		    XZSC_22NumeroSiteLocal());
    }

   return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'un acquittement a TELEI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int eu_envoi_acquittement ( XDY_Eqt		va_Acqt  )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
	static char *version = "@(#)erau_tra.c	1.33	1.33 12/01/11      : eu_envoi_acquittement" ;

	/* Acquitement telei */   
	if (!strcmp(vg_donsysrau.Config.AdresseIpEsclave, XZECC_IP_VIDE))
	{
		EnvoyerTrame(	"\0",
						va_Acqt,
						vg_donsysrau.Config.AdresseRGS,
						XZEXC_NOCMD,
						XZEXC_PRIORITEFAIBLE,
						vg_donsysrau.SocketEsclave);
	}
	else
	{
		EnvoyerTrameIP(	"\0",
						va_Acqt,
						vg_donsysrau.Config.AdresseIpEsclave,
						vg_donsysrau.Config.PortIpEsclave,
						vg_donsysrau.Config.AdresseRGS,
						XZEXC_NOCMD,
						XZEXC_PRIORITEFAIBLE,
						vg_donsysrau.SocketEsclave);
	}
	return ( XDC_OK );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'un acquittement a TELEI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int eu_traite_bascul_rau ( char *va_zone, char *va_etat)
/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
T_Liste_Zones *pl_Zones = NULL;
XDY_Datagroup vl_dg;
int vl_update;

		  /*mise a jour de la memoire*/
		  pl_Zones = pg_Zones;
		  vl_update=0;
		  while (pl_Zones!=NULL) {
		    if (!strcmp(pl_Zones->zone.zone,va_zone)) {
		      if (strcmp(pl_Zones->zone.etat, va_etat)) {
			/*changement d etat de la zone*/
			strcpy(pl_Zones->zone.etat, va_etat);
			vl_update=1;
			break;
	              }
		      break;
		    }
		    pl_Zones = pl_Zones->suivant;
		  }
		  if (vl_update) {
		    /*mise a jour de la base*/
		    XZAO911_Ecrire_Zone_RAU(va_zone,va_etat);

		    /*maj des IHM par envoi du poke COM_CANAL_TRAU_BASCUL*/
		    XDG_EncodeDG2(vl_dg,XDG_IAX_TEXTE,XDC_NOM_SITE_CI);
		    if (!TipcSrvMsgWrite(vl_dg,
			        XDM_IdentMsg(XDM_IAX_SENDPOKE),
        			XDM_FLG_SRVMSGWRITE,
        			T_IPC_FT_INT4, COM_SENDPOKE, 
        			T_IPC_FT_INT4, COM_CANAL_TRAU_BASCUL,           
        		NULL)) {
  				XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IAX_SENDPOKE");
		    }
		    XDG_EncodeDG2(vl_dg,XDG_IAX_TEXTE,vg_NomSite); 
		    if (!TipcSrvMsgWrite(vl_dg,
			        XDM_IdentMsg(XDM_IAX_SENDPOKE),
        			XDM_FLG_SRVMSGWRITE,
        			T_IPC_FT_INT4, COM_SENDPOKE, 
        			T_IPC_FT_INT4, COM_CANAL_TRAU_BASCUL,           
        		NULL)) {
  				XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message XDM_IAX_SENDPOKE");
		    }
		}
   return ( XDC_OK );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de l'etat d'un PAU
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int eu_envoi_etat_rau_sono (ERAU_DONNEES_RAU_SONO va_donnes_PAU_SONO,
				int	va_NoCmd)

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
   static char *version = "@(#)erau_tra.c	1.33	1.33 12/01/11      : eu_envoi_etat_pau" ;

   /*B Envoi du message d'etat */
   XDY_Datagroup	pl_NomDG;
   XDG_EncodeDG2 ( pl_NomDG, XDG_ETAT_SONO,vg_NomSite);

   /* On a pas ces donnï¿½es sur le retour*/
   va_donnes_PAU_SONO.Autoroute[0]='\0';
   va_donnes_PAU_SONO.Sens=0;
   va_donnes_PAU_SONO.PR_debut=0;
   va_donnes_PAU_SONO.PR_fin=0;

   XZST_03EcritureTrace(XZSTC_WARNING, "RAU SONO ETAT : Message:%s, Autoroute:%s, Sens:%d, PRd:%d, PRf:%d, ERR=%d, PILOTE:%d, Cmd:%d",
							va_donnes_PAU_SONO.Message,
							va_donnes_PAU_SONO.Autoroute,
							va_donnes_PAU_SONO.Sens,
							va_donnes_PAU_SONO.PR_debut,
							va_donnes_PAU_SONO.PR_fin,
							va_donnes_PAU_SONO.Nb_PAUErreur,
							va_donnes_PAU_SONO.Nb_PAUPilotes,
							va_NoCmd);
   if (!TipcSrvMsgWrite ( pl_NomDG,
			  XDM_IdentMsg(XDM_ETAT_SONO),
						XDM_FLG_SRVMSGWRITE
						,T_IPC_FT_STR,  va_donnes_PAU_SONO.Message
						,T_IPC_FT_STR,  va_donnes_PAU_SONO.Autoroute
						,T_IPC_FT_INT4, va_donnes_PAU_SONO.Sens
						,T_IPC_FT_INT4, va_donnes_PAU_SONO.PR_debut
						,T_IPC_FT_INT4, va_donnes_PAU_SONO.PR_fin
						,T_IPC_FT_INT4, va_donnes_PAU_SONO.Nb_PAUErreur
						,T_IPC_FT_INT4, va_donnes_PAU_SONO.Nb_PAUPilotes
						,T_IPC_FT_INT4, va_NoCmd,
					  NULL))
  {
	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_RAU_SONO non effectue." );
	return (XDC_NOK);
  }
  else
  {
	XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_RAU_SONO effectue");
  }

  return ( XDC_OK );
}

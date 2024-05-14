/*E*/
/* Fichier : $Id: epma_tra.c,v 1.12 2019/01/29 11:03:53 pc2dpdy Exp $        Release : $Revision: 1.12 $        Date : $Date: 2019/01/29 11:03:53 $
------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de traitement des trames recues de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	03/06/08	: trace WARNING --> INFO v1.5
* PNI	27/08/08	: Envoi alerte et action à OK sur affichage incohérent v1.6 DEM 806
* JMG	14/03/11	: correction affecte_etat_pmva pour linux 1.7
* JMG	11/11/12	: TDP
* JPL	14/10/15	: Etat PMVA : prise en compte du retour 'Neutre 3 points' (DEM 1135 complement)  1.9
* JPL	09/11/15	: Etat PMVA : Sur retour 'Neutre 3 points', conserver le TEXTE (DEM 1135 complement)  1.10
* JPL	14/02/18	: Migration Linux 64 bits (DEM 1274) : Initialisation des variables; code de retour des fonctions  1.11
* JMG	03/10/18	: IP suppression SAGA DEM1306 1.12
* JMG	29/01/19	: correction bug 1.13
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"
#include "xzma.h"

#include "epma_tra.h"

/* definitions de constantes */

#define CM_SEPARATEUR_LIGNE	"\r\n"
#define CM_DEBUT_LIGNE		"K AM="
#define CM_DEBUT_LIGNE_IP		"AM=1."
#define CM_LEC_NUM_LIGNE	CM_DEBUT_LIGNE "%d"
#define CM_LEC_NUM_LIGNE_IP	CM_DEBUT_LIGNE_IP "%d"
#define CM_DEBUT_CONTRASTE	"EC="
#define CM_LEC_CONTRASTE	CM_DEBUT_CONTRASTE "%c"
#define CM_DEBUT_CLIGNO		"CL="
#define CM_LEC_CLIGNO		CM_DEBUT_CLIGNO "%s"
#define CM_DEBUT_AFF		"AF="
#define CM_LEC_AFF		CM_DEBUT_AFF "%s"
#define CM_LIGNE_UNE		1
#define CM_LIGNE_DEUX		2
#define CM_LIGNE_TROIS		3
#define CM_LIGNE_QUATRE		4
#define CM_LIGNE_CINQ		5
#define CM_LIGNE_SIX		6
#define CM_LIGNE_SEPT		7

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

int affecte_etat_pmva   ( char *, XDY_Etat_PMVA *, char *, XDY_Octet );


/* definition de fonctions externes */
extern void supprime_blancs_fin(char *);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi un msg d'alarme
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int recherche_eqt ( char  	*va_adrrgs,
		    XDY_Mot	va_TypeRep,
		    XDY_Mot	va_TypeTrame,
		    XDY_Mot	*va_noeqt,
		    XDY_District	*va_SiteGestion)
/*
* ARGUMENTS EN ENTREE :
*
* va_horodate : horodate de l'eqt
* va_type_err : 1 non reponse    2 err crc
* va_nb_err : nb err de l'eqt
* va_adrrgs : adresse rgs de l'eqt
*
* ARGUMENTS EN SORTIE :
*
* va_noeqt : no eqt traitÅ
*
* CODE RETOUR : 
*
* XDC_VRAI : picto
* XDC_FAUX : pmv
* 3 : eqt non trouve
* CONDITION D'UTILISATION
*
* FONCTION 
*  Envoi un msg d'alarme suivant le type d'eqt et le type de l'alarme
*
------------------------------------------------------*/
{
 static char *version = "@(#)epma_tra.c	1.38 05/31/00      :recherche_eqt " ;
 
 EPMVA_DONNEES_PMVA		*pl_ListePMVA;
 int	vl_typeeqt = 0;
 char * vl_status[32];  
 
      XZST_03EcritureTrace(XZSTC_INTERFACE , "recherche eqt %s",va_adrrgs);
      /*A Recherche du pmv concern� */
      for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
	         pl_ListePMVA = pl_ListePMVA->Suivant )
      {
         if (
	     ( (!strcmp(pl_ListePMVA->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePMVA->Config.AdresseRGS,va_adrrgs))) 
	     ||
	     ((strcmp(pl_ListePMVA->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePMVA->Config.AdresseIP, va_adrrgs)))
	     )
         {
	    /* Sauvegarde donnees pmv */
	    vl_typeeqt = XZECC_TYPE_EQT_PMVA;
	    pl_ListePMVA->Etat.Numero = pl_ListePMVA->Config.Numero.NoEqt;
	    *va_noeqt  = pl_ListePMVA->Config.Numero.NoEqt;
	    XZST_03EcritureTrace(XZSTC_FONCTION,"eqt trouve %d",*va_noeqt);
	    *va_SiteGestion  = pl_ListePMVA->Config.SiteGestion;
	    strcpy(va_adrrgs, pl_ListePMVA->Config.AdresseRGS);
	    if ( (va_TypeTrame == XZEXC_TRAME_FEN_LCR) || (va_TypeRep != XZEXC_TRAME_NON_TRANSM) ) 
	       pl_ListePMVA->EtatTrans = EPMVAC_EQT_LIBRE;
	    else
	       pl_ListePMVA->EtatTrans = EPMVAC_EQT_PANNE;
	    break;
         }
      }

   return(vl_typeeqt);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork etat d'affichage PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ep_envoi_affichage_PMVA ( 	XDY_Etat_PMVA va_EtatPMVA,
				int	     va_NumEqt,
				XDY_District	va_site
				)
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
 static char *version = "@(#)epma_tra.c	1.38 05/31/00      : ep_envoi_affichage_PMV " ;
 
  XZSCT_NomTache        vl_NomTache   = "";
  XDY_NomMachine        vl_NomMachine = "";
  XDY_Datagroup		pl_DG;


	/*A Construction du datagroup */
	       
	XDG_EncodeDG2 ( pl_DG, XDG_ETAT_PMVA, XZSC23_Nom_From_Numero(va_site)) ;			

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PMVA */
	if (!TipcSrvMsgWrite (  pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_PMVA),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, va_NumEqt,
                          	T_IPC_FT_STR,  va_EtatPMVA.Texte1,
                          	T_IPC_FT_INT2, va_EtatPMVA.Affichage1,
                          	T_IPC_FT_STR,  va_EtatPMVA.Alternat1,
                          	T_IPC_FT_CHAR, va_EtatPMVA.Clign1,
                          	T_IPC_FT_STR,  va_EtatPMVA.Texte2,
                          	T_IPC_FT_INT2, va_EtatPMVA.Affichage2,
                          	T_IPC_FT_STR,  va_EtatPMVA.Alternat2,
                          	T_IPC_FT_CHAR, va_EtatPMVA.Clign2,
                          	T_IPC_FT_STR,  va_EtatPMVA.Texte3,
                          	T_IPC_FT_INT2, va_EtatPMVA.Affichage3,
                          	T_IPC_FT_STR,  va_EtatPMVA.Alternat3,
                          	T_IPC_FT_CHAR, va_EtatPMVA.Clign3,
                          	T_IPC_FT_STR,  va_EtatPMVA.Texte4,
                          	T_IPC_FT_INT2, va_EtatPMVA.Affichage4,
                          	T_IPC_FT_STR,  va_EtatPMVA.Alternat4,
                          	T_IPC_FT_CHAR, va_EtatPMVA.Clign4,
                           	T_IPC_FT_STR,  va_EtatPMVA.Texte5,
                          	T_IPC_FT_INT2, va_EtatPMVA.Affichage5,
                          	T_IPC_FT_STR,  va_EtatPMVA.Alternat5,
                          	T_IPC_FT_CHAR, va_EtatPMVA.Clign5,
                            	T_IPC_FT_CHAR, va_EtatPMVA.Flash,
                          	NULL))
	{
           XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ETAT_PMVA non effectue \n" );
	}
	else
	{
           XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ETAT_PMVA effectue sur %s\n",pl_DG );
           XZST_03EcritureTrace(XZSTC_WARNING ,"eqt %d // l1 %s // l2 %s // l3 %s",
	   			va_NumEqt,
				va_EtatPMVA.Texte1,
				va_EtatPMVA.Texte2,
				va_EtatPMVA.Texte3);
	}
	/* Mise jour compteur de marche equipement */
        XZSC_06NomTache( vl_NomTache );
        XZSC_07NomMachine( vl_NomMachine );
	if ( (va_EtatPMVA.Texte1[0] == '\0') && (va_EtatPMVA.Texte2[0] == '\0') &&
	     (va_EtatPMVA.Texte3[0] == '\0') && (va_EtatPMVA.Texte4[0] == '\0') && (va_EtatPMVA.Texte5[0] == '\0'))
	{
	   XZMA01_Maj_duree_marche (  va_NumEqt,
                                      XDC_EQT_PMVA,
                                      XDC_FAUX,
                                      vl_NomTache,
                                      vl_NomMachine );
	}
	else
	{
	   XZMA01_Maj_duree_marche (  va_NumEqt,
                                      XDC_EQT_PMVA,
                                      XDC_VRAI,
                                      vl_NomTache,
                                      vl_NomMachine );
	}
	
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Analyse de la trame >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket : no socket sur laquelle le message est arrivÅ
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*  analyse la trame et execute les actions correspondantes
*
------------------------------------------------------*/
{
   static char *version = "@(#)epma_tra.c	1.38 05/31/00      :em_traitement_trame " ;

   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame_ptr = NULL;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrrgs[10];
   int				vl_typetrame;
   int 				vl_typ_rep;
   XZEXT_MSG_SOCKET		vl_Msg, pl_trame;
   int				vl_param;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   XDY_Etat_PMVA 		vl_etatPMVA = { 0, "", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							XZEXC_TEXTE_PMVA_NON_REP, XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
   							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
    							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							XDC_PMVA_PASFLASH },
     				vl_etat2PMVA = { 0, "", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							XZEXC_TEXTE_PMVA_NON_REP, XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
   							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							XDC_PMVA_PASFLASH },
     				vl_EtatDefautPMVA = { 0, "", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							XZEXC_TEXTE_PMVA_NON_REP, XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
   							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
   							"", XDC_PMVA_TEXTE, XDC_PMVA_PASCLIGNO, "",
     							XDC_PMVA_PASFLASH };
   EPMVA_DONNEES_PMVA		*pl_ListePMVA;
   char				vl_cligno[15];
   char				vl_contraste;
   XDY_Datagroup		pl_DG;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd;   
   char                         vl_horod1[20],
                                vl_horod2[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   char * 			vl_status[32];
   XDY_Mot	                vl_noeqt;
   int 				vl_valret,vl_retour;
   int 				vl_typeeqt;
   XZEXT_TEXTE_STATUS		vl_tabstatus[16];
   int				vl_Alerte = 0;
   char   			vl_NbErr[10] = "";
  XDY_District			vl_sitegestion;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
      return (XDC_NOK);
   }
   else
   {
      XZST_03EcritureTrace( XZSTC_FONCTION, " -------------tra mes=%s",vl_Msg);
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
      					  	             
      /*A Retourne a l'appelant si simulation */
      if ( vg_Simulation ) return (XDC_OK);
      
      					  	     
      if (vl_param != 10)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket impossible mes=%s",vl_Msg);
         return (XDC_NOK);
      }
      					 
      /*A Conversion horodate */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      
      /*A Recherche l'equipement qui emet une reponse */
      if ( (vl_typeeqt = recherche_eqt ( vl_adrrgs, vl_typ_rep, vl_typetrame, &vl_noeqt, &vl_sitegestion)) == 0 )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Pmv ou Picto Non trouve en memoire mes=%s",vl_Msg);
         return (XDC_OK);
      }
	 
      /*A Traitement des alarmes transmission avec l'equipement */
      ex_traite_alarme_com ( 	vg_NomMachine,
				vl_typeeqt,
				vl_noeqt,
      				vl_typ_rep,
      				vl_nbnonrep,
      				vl_nberrcrc,
      				vl_typetrame,
      				vg_Mode_Fonct,
				vl_sitegestion);
      
      /*A On se positionne sur le debut de la trame */
      pl_trame_ptr = strstr( vl_Msg, XZEXC_SEP_TRAME)+4;
      strcpy ( pl_trame, strtok(pl_trame_ptr,XZEXC_FIN_MSG_SOCK) );
      pl_trame[strlen(pl_trame)-1] = '\0';
           
      if (pl_trame == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket impossible mes=%s",vl_Msg);
         return (XDC_NOK);
      }


      /*A Recherche du type de trame */
      XZST_03EcritureTrace( XZSTC_FONCTION,"type trame %d",vl_typetrame);
      switch(vl_typetrame)
      {
         /*A C'est une trame de reponse pour la fenetre LCR */
         case XZEXC_TRAME_FEN_LCR:
         {

            if (vl_typeeqt == XZECC_TYPE_EQT_PMVA)
            {
               ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListePMVAFLcr, vl_typ_rep );
            }
            
            break;
         } 
         /*A C'est une trame de reponse status temps reel */
         case XZEXC_TRAME_STAT_TR:
         {
               /*A Recherche du pmv concern� */
               for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
	         pl_ListePMVA = pl_ListePMVA->Suivant )
               {
		  if (pl_ListePMVA->Config.Numero.NoEqt==vl_noeqt)
                  /*if (!strcmp(pl_ListePMVA->Config.AdresseRGS,vl_adrrgs))*/
                  {
                       /*A Si le status TR est ko -> demande du status complet */
	               if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
	                  return (XDC_OK);
	                 
                       if ( ( ((*pl_trame != '@')&&(*(pl_trame+1) != '@')) && (pl_ListePMVA->StatusTR == XDC_OK) ) ||
                            ( ((*pl_trame == '@')||(*(pl_trame+1) == '@')) && (pl_ListePMVA->StatusTR != XDC_OK) ) )
                       {	XZST_03EcritureTrace( XZSTC_WARNING,"Chgt StusTR %c;%c.EnvoyerTrame ST",(*pl_trame),*(pl_trame+1));
			    if (!strcmp(pl_ListePMVA->Config.AdresseIP, XZECC_IP_VIDE))
	                    	EnvoyerTrame("ST\0",
	                    		XZEXC_TRAME_STAT_COMP,
	                    		vl_adrrgs,
	                    		XZEXC_NOCMD,
	                    		XZEXC_PRIORITEFAIBLE,
	                    		va_socket);
			    /*else
	                    	EnvoyerTrameIP("ST\0",
	                    		XZEXC_TRAME_STAT_COMP,
					pl_ListePMVA->Config.AdresseIP,
					pl_ListePMVA->Config.Port,
	                    		vl_adrrgs,
	                    		XZEXC_NOCMD,
	                    		XZEXC_PRIORITEFAIBLE,
	                    		va_socket);*/
	                    pl_ListePMVA->EtatTrans = EPMVAC_EQT_OCCUP;
                       }

		       if (pl_ListePMVA->Config.Type!=1)
                       	pl_ListePMVA->StatusTR = ((*pl_trame == '@')||(*(pl_trame+1)) == '@') ? XDC_OK : XDC_NOK;
		       else
		        pl_ListePMVA->StatusTR = XDC_OK;
		       if (pl_ListePMVA->StatusTR==XDC_NOK)
                         XZST_03EcritureTrace( XZSTC_WARNING, "Le statut du PMV passe a XDC_NOK:%c;%c.",(*pl_trame),*(pl_trame+1)); 
                       break;
                  }
               }
            break;
         }
         /*A C'est une trame de reponse etat d'affichage PMV */
         case XZEXC_TRAME_ETAT_AFF_PMVA:
         {   
            /*B Recherche du nombre de caracteres du PMV traite */      
            for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
	         pl_ListePMVA = pl_ListePMVA->Suivant )
	    {
		  if (pl_ListePMVA->Config.Numero.NoEqt==vl_noeqt)
	       /*if (!strcmp(pl_ListePMVA->Config.AdresseRGS,vl_adrrgs))*/
	       {
	          break;
	       }
	    }
            if (pl_ListePMVA == NULL)
            {
               XZST_03EcritureTrace( XZSTC_WARNING, " Pmv Non trouve en memoire mes=%s",vl_Msg);
               return (XDC_OK);
            }
      

	    if (vl_typ_rep==XZEXC_REP_BLC_FIN) {
	      strcat(pl_ListePMVA->Trame, pl_trame);
	    }
	    else if (vl_typ_rep==XZEXC_REP_BLC_INT) {
	      strcpy(pl_ListePMVA->Trame, pl_trame);
	      break;
	    }

	    /*A Envoi d'une demande ST a l'init */
	    if ( vl_nocmd == XZEXC_CMD_INIT )
            {
		    if (!strcmp(pl_ListePMVA->Config.AdresseIP, XZECC_IP_VIDE))
	          	EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
	                    XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
		  else
	          	EnvoyerTrameIP("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
			    pl_ListePMVA->Config.AdresseIP,
			    pl_ListePMVA->Config.Port,
	                    vl_adrrgs,
	                    XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	          pl_ListePMVA->EtatTrans = EPMVAC_EQT_OCCUP;
            }
	    
	    if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd != XZEXC_CMD_INIT) )
	       ex_ecriture_trace_cmd ( XDC_EQT_PMVA,
                                       pl_ListePMVA->Config.Numero.NoEqt,
                                       NULL,
                                       pl_trame,
                                       vl_nocmd );
	                
            /* Lecture vl_horodate */
            XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );
	    
            /*B on initialise les variables avec l'etat commandÅ et l'etat courant */
            /*B ceci va permetre la comparaison */        
	    memcpy(&vl_etatPMVA,&pl_ListePMVA->Etat,sizeof(XDY_Etat_PMVA));
	    memcpy(&vl_etat2PMVA,&pl_ListePMVA->Etat_Commande,sizeof(XDY_Etat_PMVA));
            
            /*B On recupere l'etat affiche sur le pmv */
	    vl_retour=affecte_etat_pmva(pl_ListePMVA->Trame,&vl_etatPMVA,&vl_contraste, pl_ListePMVA->Config.Type);
	    if (vl_retour==XDC_NOK)
	    {
               XZST_03EcritureTrace( XZSTC_WARNING, " Trame incomplete mes=%s",vl_Msg);
	       if (vl_nocmd > XZEXC_NOCMD)
	       {
	          /* envoi de CR NOK */
	          xzat250_CompteRendu_PMVA ( vl_nocmd, vl_horodate, XDC_NOK, pl_ListePMVA->Config.SiteGestion );
	          ex_ecriture_trace_cmd ( XDC_EQT_PMVA,
                                      	  pl_ListePMVA->Config.Numero.NoEqt,
                                      	  NULL,
                                      	  pl_trame,
                                      	  vl_nocmd );
                                      	  
	       /*   XZEZ03_CompteRendu_TDP(vl_nocmd,XDC_NOK,pl_ListePMVA->Config.Numero.NoEqt,vg_NomMachine);*/
	       }
	       if (vl_typ_rep==XZEXC_REP_BLC_FIN) 
		 strcpy(pl_ListePMVA->Trame,"");
	          return (XDC_OK);
	    }

	    vl_retour=affecte_etat_pmva(pl_ListePMVA->Trame,&vl_etat2PMVA,&vl_contraste, pl_ListePMVA->Config.Type);

	    /*B COMPARAISON ENTRE ETAT COURANT ET ETAT PRECEDENT */
	    if ( (memcmp(&vl_etatPMVA,&pl_ListePMVA->Etat,sizeof(XDY_Etat_PMVA))) &&
	         !(pl_ListePMVA->Etat_Service & XDC_EQT_HORS_SRV))
	    {
	       /*B Copie du nouvel etat d'affichage pmv ds l'etat courant */
	       memcpy(&pl_ListePMVA->Etat,&vl_etatPMVA,sizeof(XDY_Etat_PMVA));
	       
	       /*A Envoi de l'etat d'affichage du PMV */
	       ep_envoi_affichage_PMVA ( vl_etatPMVA, pl_ListePMVA->Config.Numero.NoEqt,
						pl_ListePMVA->Config.SiteGestion);
	    }
	    
	    /*B COMPARAISON ENTRE ETAT COURANT ET ETAT COMMANDE */
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd deb:%s.",pl_ListePMVA->Etat_Commande.Texte1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Texte2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Texte3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Texte4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Texte5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Alternat1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Alternat2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Alternat3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Alternat4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",pl_ListePMVA->Etat_Commande.Alternat5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Affichage1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Affichage2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Affichage3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Affichage4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Affichage5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Clign1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Clign2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Clign3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Clign4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Clign5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",pl_ListePMVA->Etat_Commande.Flash);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Texte1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Texte2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Texte3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Texte4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Texte5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Alternat1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Alternat2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Alternat3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Alternat4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%s.",vl_etat2PMVA.Alternat5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Affichage1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Affichage2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Affichage3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Affichage4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Affichage5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Clign1);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Clign2);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Clign3);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Clign4);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd:%d",vl_etat2PMVA.Clign5);
	    XZST_03EcritureTrace(XZSTC_DEBUG1,"etat cmd fin:%d",vl_etat2PMVA.Flash);
	    if ( memcmp(&vl_etat2PMVA,&pl_ListePMVA->Etat_Commande,sizeof(XDY_Etat_PMVA)) &&
	         (pl_ListePMVA->NumDerCmd == vl_nocmd) )
	    {
 /* c'est different */ /*%d %d %d",vl_nocmd,pl_ListePMV->NumCmd,pl_ListePMV->NumDerCmd);*/
	       /* filtrage de l'alerte si eqt hs ou inactif */
	       if ( !(pl_ListePMVA->Etat_Service) &&
		    (vl_nocmd != XZEXC_NOCMD) &&
	            (vl_nocmd != XZEXC_CMD_INIT) && 
	            (vl_nocmd != XZEXC_CMD_HORS_BASE))
	       {
	           XZST_03EcritureTrace(XZSTC_DEBUG1,"/* envoi de CR NOK */");
		  /*if (pl_ListePMV->NumCmd != 0) {*/
                  ex_env_alerte  ( 	vl_horodate,
                                  	XDC_EQT_PMVA,
                                      	pl_ListePMVA->Config.Numero.NoEqt,
                                      	XZAAC_AFF_DIF,
                                      	vg_Mode_Fonct,
					pl_ListePMVA->Config.SiteGestion,
                                      	"affichage PMVA",
                                      	NULL );
	          xzat250_CompteRendu_PMVA ( vl_nocmd, vl_horodate, XDC_OK , pl_ListePMVA->Config.SiteGestion);
         	  XZST_03EcritureTrace(XZSTC_WARNING ,"Etat AFF2 pmva <> etat comm -> CR OK pour cmd:%d",vl_nocmd);
	          memcpy ( &pl_ListePMVA->Etat_Commande, &vl_etat2PMVA, sizeof(XDY_Etat_PMVA) );
		  /*}*/
	       }
	/*       if ( vl_nocmd != XZEXC_CMD_INIT )
	          XZEZ03_CompteRendu_TDP(vl_nocmd,XDC_NOK,pl_ListePMVA->Config.Numero.NoEqt,vg_NomMachine);*/
	        
	       if ( !(pl_ListePMVA->Etat_Service) &&
	            (vl_nocmd != XZEXC_CMD_INIT) && 
		    (pl_ListePMVA->NumCmd <= XZEXC_NOCMD) )
	       {
		  if (pl_ListePMVA->NumCmd!=0) {
                  ex_env_alerte  ( 	vl_horodate,
                                  	XDC_EQT_PMVA,
                                      	pl_ListePMVA->Config.Numero.NoEqt,
                                      	XZAAC_AFF_DIF,
                                      	vg_Mode_Fonct,
					pl_ListePMVA->Config.SiteGestion,
                                      	"affichage PMVA",
                                      	NULL );
	          memcpy ( &pl_ListePMVA->Etat_Commande, &vl_etat2PMVA, sizeof(XDY_Etat_PMVA) );
		  }
	       }
	    }
	    else
	    {
XZST_03EcritureTrace(XZSTC_DEBUG1 ,"OK?:%d,cmd:%d",pl_ListePMVA->Etat_Service,vl_nocmd);
	       if ( !(pl_ListePMVA->Etat_Service) &&
		    (vl_nocmd != XZEXC_NOCMD) &&
	            (vl_nocmd != XZEXC_CMD_INIT) && 
	            (vl_nocmd != XZEXC_CMD_HORS_BASE))
	       {
	          XZST_03EcritureTrace(XZSTC_DEBUG1,"/* c'est pareil */");
	          /* envoi de CR OK */
	          xzat250_CompteRendu_PMVA ( vl_nocmd, vl_horodate, XDC_OK, pl_ListePMVA->Config.SiteGestion );
	      /*    XZEZ03_CompteRendu_TDP(vl_nocmd,XDC_OK,pl_ListePMVA->Config.Numero.NoEqt,vg_NomMachine);*/
         	  XZST_03EcritureTrace(XZSTC_WARNING ,"Etat AFF3 pmva = etat comm -> CR OK pour cmd:%d",vl_nocmd  );
		  if ( vl_nocmd < XZEXC_NOCMD )
		     pl_ListePMVA->NumCmd = XZEXC_NOCMD;
		  else
		     pl_ListePMVA->NumCmd = vl_nocmd;
	       } 	       
	    }
	    
	    
	    /*B si le Contraste a change -> envoi du message XDM_ETAT_Contraste  */
	    if ( ((vl_contraste == EPMVAC_CONTRASTE) && (pl_ListePMVA->Contraste==XDC_CONTRASTENORMAL)) ||
	         ((vl_contraste != EPMVAC_CONTRASTE) && (pl_ListePMVA->Contraste==XDC_CONTRASTEBRILLANT)) )
	    {
	       pl_ListePMVA->Contraste = ( vl_contraste == EPMVAC_CONTRASTE ) ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL;
	       /* Envoi XDM_Etat_Contraste  */
	       /* Construction du datagroup */
	       if ( !(pl_ListePMVA->Etat_Service & XDC_EQT_HORS_SRV) )
	       {
	          XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite) ;			
                  if (!TipcSrvMsgWrite (  pl_DG
      			  ,XDM_IdentMsg(XDM_ETAT_Contraste)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, XDC_EQT_PMVA
                          ,T_IPC_FT_INT2, pl_ListePMVA->Config.Numero.NoEqt
                          ,T_IPC_FT_INT2, pl_ListePMVA->Contraste
                          ,NULL))
      	          {
  		     /* Trace envoi message XDM_ETAT_Contraste non effectue */    	
         	     XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_Contraste non effectue \n");
      	          }
      	          else
      	          {
  		     /* Trace envoi message XDM_ETAT_Contraste effectue */    	
         	     XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_Contraste effectue \n");
         	  }
      	       }
	    }
	    
            if ( (vl_nocmd  == XZEXC_CMD_HORS_BASE) && 
                 (vl_nocmd  != XZEXC_CMD_INIT) && 
                 !(pl_ListePMVA->Etat_Service) &&
                 (pl_ListePMVA->ContrasteCmd =! pl_ListePMVA->Contraste) )
            {
               XZST_03EcritureTrace(XZSTC_WARNING, "Appel ex_env_alerte");
               ex_env_alerte  ( vl_horodate,
                                XDC_EQT_PMVA,
                                pl_ListePMVA->Config.Numero.NoEqt,
                                XZAAC_AFF_DIF,
                                vg_Mode_Fonct,
				pl_ListePMVA->Config.SiteGestion,
                                "Contraste PMVA",
                                NULL );
            }
	    if ( vl_nocmd == XZEXC_CMD_INIT )
	    {
	       memcpy(&pl_ListePMVA->Etat,&vl_etatPMVA,sizeof(XDY_Etat_PMVA));
	       memcpy(&pl_ListePMVA->Etat_Commande,&vl_etatPMVA,sizeof(XDY_Etat_PMVA));
	    }	
	    if (vl_typ_rep==XZEXC_REP_BLC_FIN) 
		 strcpy(pl_ListePMVA->Trame,"");
              break;
         } 
	 /* c'est une trame de status complet */
	 case XZEXC_TRAME_TEST_COM:
	 case XZEXC_TRAME_STAT_COMP:
	 {

               /*A Recherche du pmv concern� */
               for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
	         pl_ListePMVA = pl_ListePMVA->Suivant )
               {
		  if (pl_ListePMVA->Config.Numero.NoEqt==vl_noeqt)
                   /*if (!strcmp(pl_ListePMVA->Config.AdresseRGS,vl_adrrgs))*/
                   { 
	               if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
	               {
	                  vl_EtatDefautPMVA.Numero = pl_ListePMVA->Etat.Numero;
	                  /*A Si perte de communication avec l'equipement alors envoi d'un message "blanc" */
	                  if ( (vl_typetrame == XZEXC_TRAME_TEST_COM) &&
	                       (memcmp(&vl_EtatDefautPMVA,&pl_ListePMVA->Etat,sizeof(XDY_Etat_PMVA))) )
	                  {
	                     memcpy ( &pl_ListePMVA->Etat, &vl_EtatDefautPMVA, sizeof(XDY_Etat_PMVA) );
	                     ep_envoi_affichage_PMVA ( pl_ListePMVA->Etat, pl_ListePMVA->Config.Numero.NoEqt ,
						pl_ListePMVA->Config.SiteGestion);
	                  }
	                  return (XDC_OK);
	               }
	               else
	               {
	                  if ( vl_typetrame == XZEXC_TRAME_TEST_COM )
	                     memcpy ( &pl_ListePMVA->Etat, &vl_EtatDefautPMVA, sizeof(XDY_Etat_PMVA) );
	               }
	               break;
	           }
               }
XZST_03EcritureTrace( XZSTC_INFO,"ST:%s.", pl_trame);           
               /* Decodage de la trame de status */
               ex_decode_status ( pl_trame,
                                  vl_tabstatus,
                                  13,
                                  vl_adrrgs,
                                  vl_horodate,
                                  vl_typeeqt,
                                  pl_ListePMVA->Config.Numero.NoEqt,
                                  vg_NomMachine,
				  pl_ListePMVA->Config.SiteGestion);
/*!! On sait jamais si un jour , on traite de nouveau le ERI 
               ex_traitERI_PMV(vl_tabstatus[7],vl_horodate,vl_typeeqt,pl_ListePMVA->Config.Numero.NoEqt,vg_NomMachine, pl_ListePMVA->Config.SiteGestion);
*/

               /*A Traitement des alerte connexion locale */
               if ( (vl_tabstatus[3][0]  != '0') && (vl_tabstatus[3][0]  != ' ') && (vl_tabstatus[3][0]  != '\0') ) vl_Alerte = XZAAC_CLE;
               if ( (vl_tabstatus[11][0] != '0') && (vl_tabstatus[11][0] != ' ') && (vl_tabstatus[11][0] != '\0') ) vl_Alerte = XZAAC_CLL;
               if ( (vl_tabstatus[12][0] != '0') && (vl_tabstatus[12][0] != ' ') && (vl_tabstatus[12][0] != '\0') ) vl_Alerte = XZAAC_CLE;
               if ( (vl_Alerte != 0) && !(pl_ListePMVA->Etat_Service) && (pl_ListePMVA->Mode == XZEXC_MODE_NORMAL) )
                  ex_env_alerte  (    vl_horodate,
                                      vl_typeeqt,
                                      pl_ListePMVA->Config.Numero.NoEqt,
                                      vl_Alerte,
                                      vg_Mode_Fonct,
				      pl_ListePMVA->Config.SiteGestion,
                                      (vl_Alerte == XZAAC_CLL) ? vl_tabstatus[11] : (vl_tabstatus[12][0] !=  ' ') ? vl_tabstatus[12] : vl_tabstatus[3],
                                      NULL );
               if ( vl_Alerte == 0 )
                  pl_ListePMVA->Mode = XZEXC_MODE_NORMAL;
               else
                  pl_ListePMVA->Mode = XZEXC_MODE_CONNECTION_LOCALE;
   
               XZST_03EcritureTrace( XZSTC_DEBUG1, " Decodage status PMV" );
            }
      }
   }

   return (XDC_OK);
}

int detect_heure(char * va_texte)
{
char *vl_position;

   vl_position=strstr(va_texte,":");
   
   if (vl_position==NULL) return (XDC_FAUX);
   
   if ((isdigit(*(vl_position-1))) &&
       (isdigit(*(vl_position+1))) &&
       (isdigit(*(vl_position+2))))
   {
      return (XDC_VRAI);
   }
   else
   {
      return (XDC_FAUX);
   }
}

int detect_neutre_3points(char * va_texte)
{
char *vl_position;

   vl_position=strstr(va_texte,"          ...");
   
   if (vl_position==NULL) return (XDC_FAUX);
   
   return (XDC_VRAI);
}

int detect_temp(char * va_texte)
{
char *vl_position;

   vl_position=strstr(va_texte,"³C");
   
   if (vl_position==NULL) return (XDC_FAUX);
   
   if (isdigit(*(vl_position-1)))
   {
      return (XDC_VRAI);
   }
   else
   {
      return (XDC_FAUX);
   }
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int affecte_etat_pmva(char *pa_trame,
                       XDY_Etat_PMVA *pa_etatPMV,   
                       char			*va_contraste,
		       XDY_Octet	va_type)
/*
* ARGUMENTS EN ENTREE :

*  *pa_trame : trame en entree
*  *pa_etatPicto : etat de l'affichage du PMV
*  *pa_ListeTypePMV : donnees type PMV
*  *pa_contraste : extraction de contraste
*  va_NbLigne    : nombre de ligne du PMV
*   
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*  *pa_etatPMV : etat de l'affichage du picto
*
* CONDITION D'UTILISATION
*
* FONCTION 
*  extrait les infos et les affecte dans pa_etatPMV
*
------------------------------------------------------*/
{
char		vl_TrameTrav[512];
char		vl_TrameTrav2[512];
char * 		pl_Trame[12] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
char		vl_cligno[15];
char		vl_cligno2[15];
char *		vl_Mot;
char		vl_Texte[32], vl_Alter[32];
char		vl_Contraste;
int		vl_Index = 1,
		vl_Aff = 0,
		vl_Clig = 0,
		vl_Ligne = 0;

static char *version = "@(#)epma_tra.c	1.38 05/31/00      :affecte_etat_pmv " ;

	/* Decoupage de la trame en ligne */
	XZST_03EcritureTrace( XZSTC_INTERFACE, "affecte etat '%s' %d",pa_trame+1,va_type);
	
	if (strlen(pa_trame)==0) {
	  XZST_03EcritureTrace( XZSTC_WARNING, "trame VIDE");
	  return (XDC_NOK);
	}
	if (va_type!=1) {
		strcpy ( vl_TrameTrav, pa_trame );
		pl_Trame[vl_Index++] = strtok( vl_TrameTrav, CM_SEPARATEUR_LIGNE );
		while ( (pl_Trame[vl_Index++]=strtok(NULL, CM_SEPARATEUR_LIGNE )) != NULL );

	}
	else {
		strcpy ( vl_TrameTrav, pa_trame+1 );
		pl_Trame[vl_Index++] = strstr(vl_TrameTrav, "AM=");
		while ((pl_Trame[vl_Index] = strstr(pl_Trame[vl_Index-1]+3, "AM="))!=NULL)  {
			vl_Index++;
		}
	}

	for ( vl_Index = 1; vl_Index < 7; vl_Index++ ) 
	{
	   XZST_03EcritureTrace( XZSTC_FONCTION,"pl_Trame[vl_Index] %d  = %s",vl_Index, pl_Trame[vl_Index]);
	   if (va_type==1) {
	   	if ( (pl_Trame[vl_Index]==NULL) || ((vl_Mot=strstr(pl_Trame[vl_Index],"AM="))==NULL) ) break;
	   }
	   else {
	   	if ( (pl_Trame[vl_Index]==NULL) || ((vl_Mot=strstr(pl_Trame[vl_Index],CM_DEBUT_LIGNE))==NULL) ) break;
	   }

      	   /* Extraction du numero de ligne  */
	   if (va_type==1) {
	   if ( sscanf(vl_Mot,CM_LEC_NUM_LIGNE_IP, &vl_Ligne) !=1 ) 
      	   {
	      if (sscanf(vl_Mot,"AM=%d", &vl_Ligne) !=1 ) {
      	      XZST_03EcritureTrace( XZSTC_WARNING, "Syntaxe etat affichage PMV incorrecte %s", vl_Mot);
      	      return(XDC_NOK);
	     }
		vl_Ligne=vl_Ligne+2;
      	   }
		vl_Ligne++;
	  }
	  else {
	   if ( sscanf(vl_Mot,CM_LEC_NUM_LIGNE, &vl_Ligne) !=1 ) 
      	   {
      	      XZST_03EcritureTrace( XZSTC_WARNING, "Syntaxe etat affichage PMV incorrecte %s", vl_Mot);
      	      return(XDC_NOK);
      	   }
	  }

	   XZST_03EcritureTrace( XZSTC_WARNING, "ligne %d",vl_Ligne);
	   if ( (vl_Mot=strstr(pl_Trame[vl_Index], CM_DEBUT_CLIGNO)) != NULL )
	   {
      	      if (sscanf(vl_Mot,CM_LEC_CLIGNO,vl_cligno)!=1) 
      	      {
      	         XZST_03EcritureTrace( XZSTC_WARNING, "clignotement 1er ligne incorrecte");
      	         return(XDC_NOK);
      	      }
      	      sprintf ( vl_cligno2, "CL=%s", vl_cligno );
	      if (!strcmp(vl_cligno2,vg_clignolent)) 
	      {
	         vl_Clig = XDC_PMVA_CLIGNOLENT;
	      }
	      else
	      {
	         if (!strcmp(vl_cligno2,vg_clignorapide))
	         {
	            vl_Clig = XDC_PMVA_CLIGNORAPIDE;
	         }
	         else 
	         {
	            vl_Clig = XDC_PMVA_PASCLIGNO;
	         }   
	      }
	   }
	   else
	   {
	      vl_Clig = XDC_PMVA_PASCLIGNO;
	   }

	   if ( (vl_Mot=strstr(pl_Trame[vl_Index], CM_DEBUT_CONTRASTE)) != NULL )
	   {
	      sscanf ( vl_Mot, CM_LEC_CONTRASTE, &vl_Contraste );
	   }

      	   /* Extraction du texte et de l'alternatif eventuel  */
      	   strcpy ( vl_Texte, "" );
      	   strcpy ( vl_Alter, "" );
	   strcpy(vl_TrameTrav2, pl_Trame[vl_Index]);
	   if ( (vl_Mot=strtok(vl_TrameTrav2, "\"" )) != NULL )
	   {
	      strcpy ( vl_Texte, ( (vl_Mot=strtok(NULL, "\"" )) == NULL ) ? "\0" : vl_Mot );
	      vl_Mot = strtok ( NULL, "\"" );	      
	      if ((vl_Mot != NULL) && ( *vl_Mot == '/' )) /*correction LINUX */
	         strcpy ( vl_Alter, ( (vl_Mot=strtok(NULL, "\"" )) == NULL ) ? "\0" : vl_Mot );
	      else
	         strcpy ( vl_Alter, "" );
	   }
	   XZST_03EcritureTrace( XZSTC_WARNING, "vl_Texte %s",vl_Texte);
	   vl_Aff = XDC_PMVA_TEXTE;
	   /* Detection de l'heure */
	   if(detect_heure(vl_Texte))
	   {
      	       vl_Aff = XDC_PMVA_HEURE;
      	       strcpy ( vl_Texte, "" );
      	       strcpy ( vl_Alter, "" );
      	   }
	       
	   /* Detection du neutre */
	   if(detect_neutre_3points(vl_Texte))
	   {
	      /* Conserver le texte et son type */
	      /* vl_Aff = XDC_PMVA_3POINTS; */
      	   }

	supprime_blancs_fin(vl_Texte);
	 	       
	   
	 
	   switch ( vl_Ligne )
	   {
	      case CM_LIGNE_UNE :
	            strcpy ( pa_etatPMV->Texte1,    vl_Texte );
	            strcpy ( pa_etatPMV->Alternat1, vl_Alter );
	            pa_etatPMV->Affichage1 = vl_Aff;
	            pa_etatPMV->Clign1     = vl_Clig;

	         break;
	      case CM_LIGNE_DEUX :
	            strcpy ( pa_etatPMV->Texte2,    vl_Texte );
	            strcpy ( pa_etatPMV->Alternat2, vl_Alter );
	            pa_etatPMV->Affichage2 = vl_Aff;
	            pa_etatPMV->Clign2     = vl_Clig;
	         break;
	      case CM_LIGNE_TROIS :
	            strcpy ( pa_etatPMV->Texte3,    vl_Texte );
	            strcpy ( pa_etatPMV->Alternat3, vl_Alter );
	            pa_etatPMV->Affichage3 = vl_Aff;
	            pa_etatPMV->Clign3     = vl_Clig;
	            *va_contraste = vl_Contraste;
	         break;
	      case CM_LIGNE_QUATRE :
	            strcpy ( pa_etatPMV->Texte4,    vl_Texte );
	            strcpy ( pa_etatPMV->Alternat4, vl_Alter );
	            pa_etatPMV->Affichage4 = vl_Aff;
	            pa_etatPMV->Clign4     = vl_Clig;
	            *va_contraste = vl_Contraste;
	         break;
	      case CM_LIGNE_CINQ :
	            strcpy ( pa_etatPMV->Texte5,    vl_Texte );
	            strcpy ( pa_etatPMV->Alternat5, vl_Alter );
	            pa_etatPMV->Affichage5 = vl_Aff;
	            pa_etatPMV->Clign5     = vl_Clig;
	            *va_contraste = vl_Contraste;
	         break;
	      case CM_LIGNE_SIX :
	         /*A Extration eventuelle du flash */
	      /*   if ( pa_etatPMV->Flash == XDC_PMV_FLASHPRESENT )
	         {*/
		 if ( (vl_Mot=strstr(pl_Trame[vl_Index], "AF=")) != NULL )
	         {
	            char	vl_flashC;
		    /*sscanf ( vl_Mot, "AF=%c", &vl_flashC );*/
		    vl_flashC=vl_Mot[3];
      	            if ( vl_flashC == '1' ) pa_etatPMV->Flash = XDC_PMV_FLASH;
	            else 		       pa_etatPMV->Flash = XDC_PMV_PASFLASH;
		 }
	      /*   }*/
	         break;
	      case CM_LIGNE_SEPT :
	         break;
	      default :
      	         XZST_03EcritureTrace( XZSTC_WARNING, "Numero de ligne affichage PMVA incorrecte %d", vl_Ligne);
      	         return(XDC_NOK);
	         break;
	   } /*fin du switch sur vl_ligne*/
	} /*fin du for*/

	
        if ( (pa_etatPMV->Affichage1 == XDC_PMVA_3POINTS) ||
             (pa_etatPMV->Affichage2 == XDC_PMVA_3POINTS) ||
             (pa_etatPMV->Affichage3 == XDC_PMVA_3POINTS) ||
             (pa_etatPMV->Affichage5== XDC_PMVA_3POINTS) ||
             (pa_etatPMV->Affichage4 == XDC_PMVA_3POINTS) )
        {
           pa_etatPMV->Affichage1 = XDC_PMVA_3POINTS;
	   pa_etatPMV->Affichage2 = XDC_PMVA_3POINTS;
	   pa_etatPMV->Affichage3 = XDC_PMVA_3POINTS;
	   pa_etatPMV->Affichage5 = XDC_PMVA_3POINTS;
	   pa_etatPMV->Affichage4 = XDC_PMVA_3POINTS;
        }

        if ( (pa_etatPMV->Affichage1 == XDC_PMV_HEURE) ||
             (pa_etatPMV->Affichage2 == XDC_PMV_HEURE) ||
             (pa_etatPMV->Affichage3 == XDC_PMV_HEURE) ||
             (pa_etatPMV->Affichage5== XDC_PMV_HEURE) ||
             (pa_etatPMV->Affichage4 == XDC_PMV_HEURE) )
        {
           pa_etatPMV->Affichage1 = XDC_PMV_HEURE;
	   pa_etatPMV->Affichage2 = XDC_PMV_HEURE;
	   pa_etatPMV->Affichage3 = XDC_PMV_HEURE;
	   pa_etatPMV->Affichage5 = XDC_PMV_HEURE;
	   pa_etatPMV->Affichage4 = XDC_PMV_HEURE;
	   strcpy ( pa_etatPMV->Texte1,    "" );
	   strcpy ( pa_etatPMV->Alternat1, "" );
	   strcpy ( pa_etatPMV->Texte2,    "" );
	   strcpy ( pa_etatPMV->Alternat2, "" );
	   strcpy ( pa_etatPMV->Texte3,    "" );
	   strcpy ( pa_etatPMV->Alternat3, "" );
	   strcpy ( pa_etatPMV->Texte4,    "" );
	   strcpy ( pa_etatPMV->Alternat4, "" );
	   strcpy ( pa_etatPMV->Texte5,    "" );
	   strcpy ( pa_etatPMV->Alternat5, "" );
        }
      
	return ( XDC_OK );
	   
}

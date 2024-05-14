/*E*/
/*Fichier : $Id: epmv_cyc.c,v 1.13 2019/01/07 14:41:24 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2019/01/07 14:41:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Sep 1994	: Creation
* Nagiel.E	version 1.2	16 Dec 1994	:
* Mismer.D	version 1.3	23 Jan 1995	:
* Mismer.D	version 1.4	30 Jan 1995	:
* Mismer.D	version 1.5	09 Fev 1995	:
* Mismer.D	version 1.6	08 Mar 1995	:
* Mismer.D	version 1.7	17 Mar 1995	:
* Mismer.D	version 1.8	11 Mai 1995	: Ajout fonction de reconnexion aux taches protocoles
* Fontaine.C	version 1.9	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.10	13 Sep 1996	: Ajout periode variable en fonction NB eqt (RADT_W)
* Mismer.D      version 1.11    10 Mar 1998     : Modif pour PMV PICTO avec la meme RGS (DEM/1618)
*
* JPL		18/03/11 : Migration architecture HP ia64 (DEM 975) : retour de C.R. OK ou NOK (ey_traitement_cyclique)  1.12
* JMG		02/09/18 : PMV IP suppression SAGA DEM1306 1.13
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* GGY   15/06/23    :   Ajout EnvoyerTrameIp et ex_ret_LCR_IP (DEM472)
------------------------------------------------------*/

/* fichiers inclus */

#include "epmv_cfg.h"
#include "epmv_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: epmv_cyc.c,v 1.13 2019/01/07 14:41:24 pc2dpdy Exp $ : epmv_cyc" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


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
*  Numero de la socket appelante 
*
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
*
* CODE RETOUR : 
*  Aucun 
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
   static char *version = "$Id: epmv_cyc.c,v 1.13 2019/01/07 14:41:24 pc2dpdy Exp $ : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EPMV_DONNEES_PMV 	*pl_listepmv;
   EPMV_DONNEES_Picto 	*pl_listepicto;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   char			vl_Cas;
   int			vl_cr = XDC_OK;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   XZST_03EcritureTrace( XZSTC_FONCTION,"reveil cyclique");
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
	  XZST_03EcritureTrace( XZSTC_FONCTION,"%s",pl_Msg);
  	  /*A Si il s'agit d'un reveil cyclique */
  	  if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
  	  {
  	     sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					         &vl_Horodate,
  	  					         &vl_Delai,
  	  					         pl_CleProg );
  	  					         
  	     if (strstr(pl_CleProg,"Picto_FLcr_NoEqt:")!=NULL)
  	     { 
  	        sscanf(pl_CleProg,"Picto_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt,
                			vl_Cas,
			 		&pg_DebutListePictoFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return (XDC_OK);
  	        
  	     }

  	     if (strstr(pl_CleProg,"PMV_FLcr_NoEqt:")!=NULL)
  	     {
  	        sscanf(pl_CleProg,"PMV_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt, 
                			vl_Cas,
			 		&pg_DebutListePMVFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return (XDC_OK);
  	        
  	     }

  	     /*A Traitement du reveil 10S */
             if ( strstr ( pl_CleProg, EPMVC_TIMER_10S ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_FONCTION, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pmv et les picto -> demande de status TR */
  	            
                    for ( pl_listepmv = pg_debutPMV; pl_listepmv != NULL; pl_listepmv = pl_listepmv->Suivant )
                    {
                        if ( !(pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV) &&
                              (pl_listepmv->EtatTrans == EPMVC_EQT_LIBRE) &&
			      (!strcmp(pl_listepmv->Config.AdresseIP, XZECC_IP_VIDE)))
                        {
			    if (pl_listepmv->Config.TypePMV==7)
                            EnvoyerTrame(  	"PE AM=*",
                                         	XZEXC_TRAME_ETAT_AFF_PMV,
                                         	pl_listepmv->Config.AdresseRGS,
                                        	XZEXC_NOCMD,
	                                	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepmv->Socket);
			    else
                            EnvoyerTrame(  	"P\0",
                                         	XZEXC_TRAME_STAT_TR,
                                         	pl_listepmv->Config.AdresseRGS,
                                        	XZEXC_NOCMD,
	                                	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepmv->Socket);
                           pl_listepmv->EtatTrans = EPMVC_EQT_OCCUP;
                        }
			else if ( !(pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV) &&
				(strcmp(pl_listepmv->Config.AdresseIP, XZECC_IP_VIDE))) {
				EnvoyerTrameIP(       "P\0",
						XZEXC_TRAME_STAT_TR,
						pl_listepmv->Config.AdresseIP,
						pl_listepmv->Config.Port,
						pl_listepmv->Config.AdresseRGS,
						XZEXC_NOCMD,
						XZEXC_PRIORITEFAIBLE,
						pl_listepmv->Socket);
			}
                     }  
  	            
                    for ( pl_listepicto = pg_debutPicto; pl_listepicto != NULL; pl_listepicto = pl_listepicto->Suivant )
                    {
                        if ( !(pl_listepicto->Etat_Service & XDC_EQT_HORS_SRV) &&
                              (pl_listepicto->EtatTrans == EPMVC_EQT_LIBRE) &&
                              (pl_listepicto->Config.Eqt_PMV_Picto == XDC_FAUX) )
                        {
                            if (!strcmp(pl_listepmv->Config.AdresseIP, XZECC_IP_VIDE))
                            {
                                EnvoyerTrame(	"P\0",
                                                XZEXC_TRAME_STAT_TR,
                                                pl_listepicto->Config.AdresseRGS,
                                                XZEXC_NOCMD,
                                                XZEXC_PRIORITEFAIBLE,
                                                pl_listepicto->Socket);
                            }
                            else
                            {
                                EnvoyerTrameIP( "P\0",
                                                XZEXC_TRAME_STAT_TR,
                                                pl_listepmv->Config.AdresseIP,
                                                pl_listepmv->Config.Port,
                                                pl_listepmv->Config.AdresseRGS,
                                                XZEXC_NOCMD,
                                                XZEXC_PRIORITEFAIBLE,
                                                pl_listepmv->Socket);
                            }
                            pl_listepicto->EtatTrans = EPMVC_EQT_OCCUP;
                        }
                    }  
  	     }
  	     
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, EPMVC_TIMER_1MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_FONCTION, "%s : %d", pl_CleProg, vl_Delai );
  	                  
  	            /*A Pour tous les pmv et les picto -> demande de status complet si status temps reel NOK */
  	            /*A Pour tous les pmv et les picto -> demande d'etat d'affichage (PS) */
  	            
                    for ( pl_listepmv = pg_debutPMV; pl_listepmv != NULL; pl_listepmv = pl_listepmv->Suivant )
                    {
                        /* si il y a une connection locale -> demande d'etat d'affichage */
                        if ( !(pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV) && 
                              (pl_listepmv->EtatTrans!=EPMVC_EQT_PANNE) &&
			      !strcmp(pl_listepmv->Config.AdresseIP,XZECC_IP_VIDE))
                        {
			   if (pl_listepmv->Config.TypePMV==7) {
                           if ( EnvoyerTrame(	"PE AM=*",
                                         	XZEXC_TRAME_ETAT_AFF_PMV,
                                         	pl_listepmv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepmv->Socket) == XDC_OK )
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_OCCUP;
                           }
                           else
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_PANNE;
                           }
			   }
			   else {
                           if ( EnvoyerTrame(	"PS\0",
                                         	XZEXC_TRAME_ETAT_AFF_PMV,
                                         	pl_listepmv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepmv->Socket) == XDC_OK )
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_OCCUP;
                           }
                           else
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_PANNE;
                           }

			   }
                        }
			else if (!(pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV) &&
				(pl_listepmv->EtatTrans!=EPMVC_EQT_PANNE) && 
				strcmp(pl_listepmv->Config.AdresseIP,XZECC_IP_VIDE)) 
                        {
                           if ( EnvoyerTrameIP(	"PS\0",
                                         	XZEXC_TRAME_ETAT_AFF_PMV,
                                         	pl_listepmv->Config.AdresseIP,
                                         	pl_listepmv->Config.Port,
                                         	pl_listepmv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepmv->Socket) == XDC_OK )
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_OCCUP;
                           }
                           else
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_PANNE;
                           }
                        }
			XZST_03EcritureTrace( XZSTC_FONCTION,"pl_listepmv->StatusTR %d",pl_listepmv->StatusTR);
			XZST_03EcritureTrace( XZSTC_FONCTION,"pl_listepmv->EtatTrans %d",pl_listepmv->EtatTrans);
			XZST_03EcritureTrace( XZSTC_FONCTION,"Etat_Service %d",pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV);
			XZST_03EcritureTrace( XZSTC_FONCTION,"ip %s",pl_listepmv->Config.AdresseIP);
                        if ( !(pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV) &&
			      !strcmp(pl_listepmv->Config.AdresseIP,XZECC_IP_VIDE) &&
                              ((pl_listepmv->StatusTR==XDC_NOK) || (pl_listepmv->EtatTrans==EPMVC_EQT_PANNE)) )
                        {
	                   if ( EnvoyerTrame(	"ST\0",
	                                 	(pl_listepmv->EtatTrans==EPMVC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listepmv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepmv->Socket) == XDC_OK )
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_OCCUP;
                           }
                           else
                           {
                               ec_init_cnx ();
                           }
                        }
			else if  ( !(pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV) &&
				strcmp(pl_listepmv->Config.AdresseIP,XZECC_IP_VIDE) &&
				((pl_listepmv->StatusTR==XDC_NOK) || (pl_listepmv->EtatTrans==EPMVC_EQT_PANNE)) )
                        {
	                   if ( EnvoyerTrameIP(	"ST\0",
	                                 	(pl_listepmv->EtatTrans==EPMVC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listepmv->Config.AdresseIP,
                                         	pl_listepmv->Config.Port,
                                         	pl_listepmv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepmv->Socket) == XDC_OK )
                           {
                               pl_listepmv->EtatTrans = EPMVC_EQT_OCCUP;
                           }
                           else
                           {
                               ec_init_cnx ();
                           }
                        }
                     }  
  	            
                     for ( pl_listepicto = pg_debutPicto; pl_listepicto != NULL; pl_listepicto = pl_listepicto->Suivant )
                     {
                         if ( !(pl_listepicto->Etat_Service & XDC_EQT_HORS_SRV) && 
                               (pl_listepicto->EtatTrans!=EPMVC_EQT_PANNE) &&
                               (pl_listepicto->Config.Eqt_PMV_Picto == XDC_FAUX) )
                         {
                            if (!strcmp(pl_listepmv->Config.AdresseIP, XZECC_IP_VIDE))
                            {
                                if ( EnvoyerTrame(	"PS\0",
                                                    XZEXC_TRAME_ETAT_AFF_PICTO,
                                                    pl_listepicto->Config.AdresseRGS,
                                                    XZEXC_NOCMD,
                                                    XZEXC_PRIORITEFAIBLE,
                                                    pl_listepicto->Socket) == XDC_OK )
                                {
                                    pl_listepicto->EtatTrans = EPMVC_EQT_OCCUP;
                                }
                                else
                                {
                                    pl_listepicto->EtatTrans = EPMVC_EQT_PANNE;
                                }
                            }
                            else
                            {
                                if ( EnvoyerTrameIP(	"PS\0",
                                                        XZEXC_TRAME_ETAT_AFF_PICTO,
                                                        pl_listepmv->Config.AdresseIP,
                                                        pl_listepmv->Config.Port,
                                                        pl_listepmv->Config.AdresseRGS,
                                                        XZEXC_NOCMD,
                                                        XZEXC_PRIORITEFAIBLE,
                                                        pl_listepmv->Socket) == XDC_OK )
                                {
                                    pl_listepicto->EtatTrans = EPMVC_EQT_OCCUP;
                                }
                                else
                                {
                                    pl_listepicto->EtatTrans = EPMVC_EQT_PANNE;
                                }
                            }
                         }  
                         if ( !(pl_listepicto->Etat_Service & XDC_EQT_HORS_SRV) &&
                               ((pl_listepicto->StatusTR==XDC_NOK) || (pl_listepicto->EtatTrans==EPMVC_EQT_PANNE)) &&
                               (pl_listepicto->Config.Eqt_PMV_Picto == XDC_FAUX) )
                         {
	                        if (!strcmp(pl_listepmv->Config.AdresseIP, XZECC_IP_VIDE))
                            {
                                if ( EnvoyerTrame(	"ST\0",
	                                 	            (pl_listepicto->EtatTrans==EPMVC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	        pl_listepicto->Config.AdresseRGS,
                                        	        XZEXC_NOCMD,
	                                 	            XZEXC_PRIORITEFAIBLE,
                                         	        pl_listepicto->Socket) == XDC_OK )
                                {
                                    pl_listepicto->EtatTrans = EPMVC_EQT_OCCUP;
                                }
                                else
                                {
                                    ec_init_cnx ();
                                }
                            }
                            else
                            {
                                if ( EnvoyerTrameIP(   	"ST\0",
                                                        (pl_listepicto->EtatTrans==EPMVC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                                        pl_listepmv->Config.AdresseIP,
                                                        pl_listepmv->Config.Port,
                                                        pl_listepmv->Config.AdresseRGS,
                                                        XZEXC_NOCMD,
                                                        XZEXC_PRIORITEFAIBLE,
                                                        pl_listepmv->Socket) == XDC_OK )
                                {
                                    pl_listepicto->EtatTrans = EPMVC_EQT_OCCUP;
                                }
                                else
                                {
                                    ec_init_cnx ();
                                }
                            }
                         }  
                    }  
  	       }
  	  }
	  
	  		/*A Si il s'agit d'un reveil pour synchro de la date */
	  		if ( strstr ( pl_CleProg, "PMV_Timer_Synchro" ) != NULL )
	  		{
             	/*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             	XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             	/* passage de l'annee sur 2 digit */
             	memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             	/*B Composition de la trame mise a la date et a l'heure */
/*  	     	sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             	sprintf( vl_tlcr,"DATE %s%s" , pl_listepmv->Config.IdLCR, vl_Horodatedecompo);
  	     		XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );				*/  	

  	     		/*B Pour tous les pmv et les picto -> demande de mise a l'heure */
        		for ( pl_listepmv = pg_debutPMV; pl_listepmv != NULL; pl_listepmv = pl_listepmv->Suivant )
        		{
            		sprintf( vl_tlcr,"DATE %s%s" , pl_listepmv->Config.IdLCR, vl_Horodatedecompo);
		    		XZST_03EcritureTrace( XZSTC_WARNING, "trame de date:[%s]", vl_tlcr);

	        		if (!strcmp(pl_listepmv->Config.AdresseIP, XZECC_IP_VIDE))
             			EnvoyerTrame(   vl_tlcr,
										XZEXC_TRAME_DATE,
										pl_listepmv->Config.AdresseRGS,
										XZEXC_NOCMD,
										XZEXC_PRIORITEFAIBLE,
										pl_listepmv->Socket);
	        		else
	     	    		EnvoyerTrameIP( vl_tlcr,
										XZEXC_TRAME_DATE,
										pl_listepmv->Config.AdresseIP,
										pl_listepmv->Config.Port,
										pl_listepmv->Config.AdresseRGS,
										XZEXC_NOCMD,
										XZEXC_PRIORITEFAIBLE,
										pl_listepmv->Socket);
            	} 
  	            
                for ( pl_listepicto = pg_debutPicto; pl_listepicto != NULL; pl_listepicto = pl_listepicto->Suivant )
                {
//                    sprintf( vl_tlcr,"DATE %s%s" , pl_listepicto->Config.IdLCR, vl_Horodatedecompo);
//		   			XZST_03EcritureTrace( XZSTC_WARNING, "trame de date:[%s]", vl_tlcr);

					if ( pl_listepicto->Config.Eqt_PMV_Picto == XDC_FAUX )
                    {
                    	if (!strcmp(pl_listepmv->Config.AdresseIP, XZECC_IP_VIDE))
						{
							EnvoyerTrame(   vl_tlcr,
											XZEXC_TRAME_DATE,
											pl_listepicto->Config.AdresseRGS,
											XZEXC_NOCMD,
											XZEXC_PRIORITEFAIBLE,
											pl_listepicto->Socket) ;
                    	}
                    	else
                    	{
                        	EnvoyerTrameIP( vl_tlcr,
											XZEXC_TRAME_DATE,
											pl_listepmv->Config.AdresseIP,
											pl_listepmv->Config.Port,
											pl_listepmv->Config.AdresseRGS,
											XZEXC_NOCMD,
											XZEXC_PRIORITEFAIBLE,
											pl_listepmv->Socket);
                    	}
					}
                }  

                /* Rearmement du reveil synchro date */
                ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "PMV_Timer_Synchro", va_NumSock, pl_MsgTimer )
	        }
        }
    }

    return (vl_cr);
}

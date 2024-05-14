/*E*/
/*Fichier : $Id: isbi_sco_H1.c,v 1.2 2012/05/29 10:09:27 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 10:09:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSCO * FICHIER isyn_sco_H1.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Mise a jour des courbe : Calcul des donnees de la 
*  courbe H+1 pour F de T.
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	22 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sco_H1.h"
#include "isbi_sra_rafobj.h"
/* definitions de constantes */
#define CM_BUFF_SOCK 600
#define CM_UNE_MINUTE 60


/* definitions de types locaux */


/* definition de macro locales */


/* declaration de variables locales */
char vl_msg[CM_BUFF_SOCK];

static char *version = "@(#)isyn_sco_H1.c	1.9 09/19/07 : isyn_sco_H1" ;

int 		vm_indice_HeurePrec=0;
float vm_donnees_HeurePrec[CM_NB_DONNEES_H1][CM_MAX_COURBES_FT][CM_NB_MAX_FENETRES];

static   float vm_donnees_H1_prec[CM_MAX_COURBES_FT][CM_NB_MAX_FENETRES];
static   float vm_donnees_H1[CM_NB_DONNEES_H1][CM_MAX_COURBES_FT][CM_NB_MAX_FENETRES];

extern char c_correspFenetre[];
extern XDY_Horodate vm_horodate_courbe;
extern char c_correspFenetre[CM_NB_MAX_FENETRES];
/* declaration de fonctions internes */



/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Envoyer_Donnees(
		  va_no_courbe,
		  va_horodate,
		  pa_caract,
		  va_valeur)

		  int va_no_courbe;
          XDY_Horodate  va_horodate;
		  tm_caracteristique_courbe *pa_caract;
          float va_valeur;


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
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_H1.c	1.9 09/19/07 : MSCO_Envoyer_Donnees ";
  int t;
  float vl_valeur;
  XDY_Horodate vl_horodate;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Envoyer_Donnees va_horodate = %f, va_valeur=%f" , va_horodate, va_valeur);

   //F(T)
      //   horodate = vm_HorodateSecInit[va_fenetre-1] + vm_Horodate_deltatr[va_fenetre-1]
      /*
   	* envoyer la vag
   	*/

   // envoi donnée
   sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, c_correspFenetre[pa_caract->fenetre-1], va_horodate, pa_caract->varcaract[va_no_courbe].no_station, pa_caract->varcaract[va_no_courbe].sens, pa_caract->varcaract[va_no_courbe].voie, pa_caract->varcaract[va_no_courbe].mesure, va_valeur);
   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   return ;
   }

   vl_horodate = va_horodate + 360;

   /*JM*/
/*
   for (t=0; t < CM_NB_DONNEES_H1 ; t++ ){
	   vl_valeur = vm_donnees_H1[t][va_no_courbe][pa_caract->fenetre-1];
	   sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE, c_correspFenetre[pa_caract->fenetre-1], vl_horodate, pa_caract->varcaract[va_no_courbe].no_station, pa_caract->varcaract[va_no_courbe].sens, pa_caract->varcaract[va_no_courbe].voie, pa_caract->varcaract[va_no_courbe].mesure, vl_valeur);
	   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	   {
		   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
		   return ;
	   }
	   vl_horodate = vl_horodate + CM_COEF_AFF * CM_UNE_MINUTE;

   }
*/
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Envoyer_Donnees_H1 ");
   return XDC_OK;
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Calcul_Donnees_H1(
          va_donnee_entree0,
          va_donnee_entree59,
          pa_donnee_sortie)
          
          float  va_donnee_entree0;
          float  va_donnee_entree59;
          float *pa_donnee_sortie;
          

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
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_H1.c	1.9 09/19/07 : MSCO_Calcul_Donnees_H1 ";
  int i;
  int j;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Calcul_Donnees_H1 va_donnee_entree0 : %f; va_donnee_entree59 : %f" , va_donnee_entree0,va_donnee_entree59 );

   *pa_donnee_sortie = vm_coeff_predictif*va_donnee_entree0+(1-vm_coeff_predictif)*va_donnee_entree59;
       
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Calcul_Donnees_H1 *pa_donnee_sortie:%f" ,*pa_donnee_sortie);
   return XDC_OK;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_MemoDernieresDonnees(
          pa_caract)
          
          tm_caracteristique_courbe *pa_caract;
          

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
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_H1.c	1.9 09/19/07 : MSCO_MemoDernieresDonnees" ;
   float vl_valeur=0;
   char vl_nom_vag[256];
   int i=0;

//   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_MemoDernieresDonnees" );
   for ( i = 0; i < pa_caract->nb_courbes; i++)
      {
      sprintf ( vl_nom_vag , "%d_%d_%d_%d" , pa_caract->varcaract[i].no_station
                                           , pa_caract->varcaract[i].sens
                                           , pa_caract->varcaract[i].voie
                                           , pa_caract->varcaract[i].mesure );
      if ( MSCO_Valeur_vag(vl_nom_vag,&vl_valeur) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de Memoriser la vag %s" , vl_nom_vag);
         return XDC_NOK;
         }
      
      vm_donnees_HeurePrec[vm_indice_HeurePrec][i][pa_caract->fenetre-1]=vl_valeur;
                                       
      }
   vm_indice_HeurePrec++;
   vm_indice_HeurePrec =  vm_indice_HeurePrec % (CM_NB_DONNEES_H1);
   
//   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_MemoDernieresDonnees" );
}


MSCO_Maj_Courbe_H1(
			pa_caract)

			tm_caracteristique_courbe *pa_caract;

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
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_H1.c	1.9 09/19/07 : MSCO_Maj_courbe_H1" ;
   char vl_nom_var[256];
   int i=0;
   int j=0;
   int t=0;
   float vl_valeur=0.0;
   float tl_valeur[CM_MAX_COURBES_FT];
   float vl_start=0, vl_incr=0;
   int vl_ind_hp59,vl_ind_hp;
   XDY_Horodate vl_horodate;

   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Maj_courbe_H1" );
   
   /*A Svg des donnees dernieres donnees recues */
   for ( j=0; j < pa_caract->nb_courbes ; j++)
      {
      sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station , 
                                               pa_caract->varcaract[j].sens , 
                                               pa_caract->varcaract[j].voie , 
                                               pa_caract->varcaract[j].mesure );
                                               
      if ( MSCO_Valeur_vag( vl_nom_var , &tl_valeur[j]) != XDC_OK )
           {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la derniere valeur %s" ,vl_nom_var);
            return XDC_NOK;
           }
      }

   
   /*A Reset du graphe H+1 */
   vl_start++;
   
   if ( vm_indice_HeurePrec > 0 )
      vl_ind_hp59=vm_indice_HeurePrec-1;
   else
      vl_ind_hp59=CM_NB_DONNEES_H1-1 ;
   vl_ind_hp=  vm_indice_HeurePrec;
   for (t=0; t < CM_NB_DONNEES_H1 ; t++ )
      {
      /*A Mise a jour des vag des var des courbes */
      for ( j=0; j < pa_caract->nb_courbes ; j++)
         {
         sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station , 
                                               pa_caract->varcaract[j].sens , 
                                               pa_caract->varcaract[j].voie , 
                                               pa_caract->varcaract[j].mesure );
                                               
         if ( MSCO_Valeur_vag( vl_nom_var , &vl_valeur) != XDC_OK )
           {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la derniere valeur %s" ,vl_nom_var);
            return XDC_NOK;
           }
        
         if ( t==0)
         MSCO_Calcul_Donnees_H1( vm_donnees_HeurePrec[vl_ind_hp][j][pa_caract->fenetre-1],
                                 vm_donnees_HeurePrec[vl_ind_hp59][j][pa_caract->fenetre-1],
                                 &vl_valeur );
         else
         MSCO_Calcul_Donnees_H1( vm_donnees_HeurePrec[vl_ind_hp][j][pa_caract->fenetre-1],
                                 vm_donnees_H1_prec[j][pa_caract->fenetre-1],
                                 &vl_valeur );
         
                  
         MSCO_Maj_vag( vl_nom_var , vl_valeur );

         vm_donnees_H1_prec[j][pa_caract->fenetre-1]=vl_valeur;

         vm_donnees_H1[t][j][pa_caract->fenetre-1]=vl_valeur;

         //todo : MAJ courbe F(T) H1 <<OUI>>
         /*
               envoyer val vl_valeur pour courbe :
               pa_caract->varcaract[j].no_station ,
               pa_caract->varcaract[j].sens ,
               pa_caract->varcaract[j].voie ,
               pa_caract->varcaract[j].mesure
               pour Horodate = ????

         */
/*
         XZST_03EcritureTrace(   XZSTC_DEBUG1 , "valeur extrapolee %f" ,vl_valeur);

      	   vl_horodate = vm_HorodateSecInit[va_fenetre-1] + CM_6HEURES + vm_Horodate_deltatr[va_fenetre-1] + ((t+1) * CM_UNE_MINUTE* CM_COEF_AFF);
		  // envoi donnée
         	 sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f",
					CMD_FLASH_DONNEES_COURBE,
					CM_COURBE_FT_REEL,
					c_correspFenetre[pa_caract->fenetre-1],
					vl_horodate,
					pa_caract->varcaract[j].no_station,
					pa_caract->varcaract[j].sens,
					pa_caract->varcaract[j].voie,
					pa_caract->varcaract[j].mesure,
					vl_valeur);

         	 if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
         	 {
         		 XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
         		 return ;
         	 }
*/
         }
      
      vl_ind_hp++;
      vl_ind_hp = vl_ind_hp % (CM_NB_DONNEES_H1);
        
      }
      
   /*A Restitution des donnees dernieres donnees recues */
   for ( j=0; j < pa_caract->nb_courbes ; j++)
      {
      sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station , 
                                               pa_caract->varcaract[j].sens , 
                                               pa_caract->varcaract[j].voie , 
                                               pa_caract->varcaract[j].mesure );
                                               
      if ( MSCO_Maj_vag( vl_nom_var , tl_valeur[j]) != XDC_OK )
           {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecter la derniere valeur %s" ,vl_nom_var);
            return XDC_NOK;
           }
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Maj_courbe_H1" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Maj_Courbe_H1_premiere(
          pa_caract)
          
          tm_caracteristique_courbe *pa_caract;
          

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
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_H1.c	1.9 09/19/07 : MSCO_Maj_courbe_H1_premiere" ;
   char vl_nom_var[256];
   int i=0;
   int j=0;
   int t=0;
   float vl_valeur=0.0;
   float tl_valeur[CM_MAX_COURBES_FT];
   float vl_start=0, vl_incr=0;
   int vl_ind_hp59,vl_ind_hp;
   XDY_Horodate vl_horodate;
   char vl_msg[256];
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Maj_courbe_H1_premiere" );
   
   /*A Svg des donnees dernieres donnees recues */
   for ( j=0; j < pa_caract->nb_courbes ; j++)
      {
      sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station , 
                                               pa_caract->varcaract[j].sens , 
                                               pa_caract->varcaract[j].voie , 
                                               pa_caract->varcaract[j].mesure );
                                               
      if ( MSCO_Valeur_vag( vl_nom_var , &tl_valeur[j]) != XDC_OK )
           {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la derniere valeur %s" ,vl_nom_var);
            return XDC_NOK;
           }
      }

   
   /*A Reset du graphe H+1 */
   
   if ( vm_indice_HeurePrec > 0 )
      vl_ind_hp59=vm_indice_HeurePrec-1;
   else
      vl_ind_hp59=CM_NB_DONNEES_H1-1 ;
   vl_ind_hp=  vm_indice_HeurePrec;
   for (t=0; t < CM_NB_DONNEES_H1 ; t++ )
      {
      /*A Mise a jour des vag des var des courbes */
      for ( j=0; j < pa_caract->nb_courbes ; j++)
         {
         sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station , 
                                               pa_caract->varcaract[j].sens , 
                                               pa_caract->varcaract[j].voie , 
                                               pa_caract->varcaract[j].mesure );
                                               
         if ( MSCO_Valeur_vag( vl_nom_var , &vl_valeur) != XDC_OK )
           {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de recuperer la derniere valeur %s" ,vl_nom_var);
            return XDC_NOK;
           }
        
         if ( t==0)
         MSCO_Calcul_Donnees_H1( vm_donnees_HeurePrec[vl_ind_hp][j][pa_caract->fenetre-1],
                                 vm_donnees_HeurePrec[vl_ind_hp59][j][pa_caract->fenetre-1],
                                 &vl_valeur );
         else
         MSCO_Calcul_Donnees_H1( vm_donnees_HeurePrec[vl_ind_hp][j][pa_caract->fenetre-1],
                                 vm_donnees_H1_prec[j][pa_caract->fenetre-1],
                                 &vl_valeur );
         
                  
         MSCO_Maj_vag( vl_nom_var , vl_valeur );

         vm_donnees_H1_prec[j][pa_caract->fenetre-1]=vl_valeur;
      
//todo : afficher données F(T) : H1 (Premiere ???)
		  XZST_03EcritureTrace(   XZSTC_DEBUG1 , "valeur extrapolee %f" ,vl_valeur);

			vl_horodate = vm_horodate_courbe + (t * 60*1.2);
		  // envoi donnée
			sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f",
					CMD_FLASH_DONNEES_COURBE,
					CM_COURBE_FT_EXTRPOLEE,
					c_correspFenetre[pa_caract->fenetre-1],
					vl_horodate,
					pa_caract->varcaract[j].no_station,
					pa_caract->varcaract[j].sens,
					pa_caract->varcaract[j].voie,
					pa_caract->varcaract[j].mesure,
					vl_valeur);
/*JM*/
/*
			if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
				return ;
			}
*/
			 }

      vl_ind_hp++;
      vl_ind_hp = vl_ind_hp % (CM_NB_DONNEES_H1);
        
      }
      

   /*A Restitution des donnees dernieres donnees recues */
   for ( j=0; j < pa_caract->nb_courbes ; j++)
      {
      sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station , 
                                               pa_caract->varcaract[j].sens , 
                                               pa_caract->varcaract[j].voie , 
                                               pa_caract->varcaract[j].mesure );
                                               
      if ( MSCO_Maj_vag( vl_nom_var , tl_valeur[j]) != XDC_OK )
           {
            XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecter la derniere valeur %s" ,vl_nom_var);
            return XDC_NOK;
           }
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Maj_courbe_H1_premiere" );
   return XDC_OK;
}



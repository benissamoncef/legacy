/*E*/
/*Fichier : $Id: isbi_sco_ft_ref_H4.c,v 1.2 2012/05/29 10:09:28 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 10:09:28 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MSCO * FICHIER isbi_sco_ft_ref_H4.c
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Courbes F(t) avec reference :
*  Gestion des donnees de reference pour la courbe H+4.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		17/09/2007 : Creation 1.1
-----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"

/* definitions de constantes */
extern char c_correspFenetre[];
#define CM_BUFF_SOCK 600


/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isbi_sco_ft_ref_H4.c	1.7 10/03/07 : isbi_sco_ft_ref_H4" ;


extern XDY_Horodate vm_Horodate_Ft_ref_H4[CM_NB_MAX_FENETRES];

extern mon_XZIST_LigneDonnees vm_donnees_H4_ref[CM_NB_MAX_FENETRES][CM_MAX_COURBES_FT_REF][XZISC_NB_LIG_MESURES];
extern mon_XZIST_LigneDonnees vm_donnees_futures[CM_NB_MAX_FENETRES][CM_MAX_COURBES_FT_REF][XZISC_NB_LIG_MESURES];
extern int vm_nb_donnees_futures[CM_NB_MAX_FENETRES];


/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise a jour periodique du graphe de reference H+4 dans une fenetre
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Raf_Courbe_H4(
          pa_caract)

          tm_caracteristique_courbe *pa_caract;


/*
* ARGUMENTS EN ENTREE :
*   va_dp : drawport
*   va_graph_H4 : objet graphe
*   pa_caract : caracteristiques des courbes du graphe
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Sur reveil par timer de rafraichissement
*
* FONCTION
*   Visualise les valeurs actuelles des donnees de reference H+4
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_H4.c	1.7 10/03/07 : MSCO_raf_courbe_H4" ;
int j, t;
	XDY_Horodate vl_horodate;
	float vl_valeur;
	char vl_msg[256];


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_raf_courbe_H4" );

   for ( j=0; j < pa_caract->nb_courbes ; j++)
   {

   for ( t=0; t < XZISC_NB_LIG_MESURES; t++ )
    {
	vl_valeur = vm_donnees_H4_ref[pa_caract->fenetre-1][j][t].Valeur;
	vl_horodate = vm_donnees_H4_ref[pa_caract->fenetre-1][j][t].Horodate;
	sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE, c_correspFenetre[pa_caract->fenetre-1], vl_horodate + (t+1)*CM_COEF_AFF, pa_caract->varcaract[j].no_station, pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, vl_valeur);
	if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	   {
	      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	 return ;
     }
																		      }
    }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_raf_courbe_H4" );
   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise a jour periodique du graphe de reference H+4 dans une fenetre
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Maj_Courbe_H4(
          pa_caract)

          tm_caracteristique_courbe *pa_caract;


/*
* ARGUMENTS EN ENTREE :
*   va_dp : drawport
*   va_graph_H4 : objet graphe
*   pa_caract : caracteristiques des courbes du graphe
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Sur reveil par timer de rafraichissement
*
* FONCTION
*   Met a jour les valeurs suivantes des donnees de reference H+4
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_H4.c	1.7 10/03/07 : MSCO_Maj_Courbe_H4" ;

   char vl_nom_var[64];
   char vl_msg[256];
   int i=0;
   int j=0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Maj_Courbe_H4" );

   /*A Mise a jour des vag des var des courbes avec la premiere donnee future */
   for ( j=0; j < pa_caract->nb_courbes ; j++)
      {
      sprintf( vl_nom_var , "Ft_ref_H4_%d_%d" , j, pa_caract->fenetre );
      if ( (vm_donnees_futures[pa_caract->fenetre-1][j][0].Validite == XDC_RADT_CORRECT) ||
           (vm_donnees_futures[pa_caract->fenetre-1][j][0].Validite == XDC_RADT_RECONST) )
         {
         MSCO_Maj_vag_horodate( vl_nom_var , (float) vm_donnees_futures[pa_caract->fenetre-1][j][0].Valeur,
						 (float) vm_donnees_futures[pa_caract->fenetre-1][j][0].Horodate);
         }
     else
         {
         MSCO_Maj_vag_horodate( vl_nom_var , (float) (-1) ,
				 (float) vm_donnees_futures[pa_caract->fenetre-1][j][0].Horodate);
         }
      }


   /*A Recadrer les donnees H+4 et futures */
   for (j=0; j < pa_caract->nb_courbes; j++ )
      {
      for (i=1;  i < CM_NB_MESURES_FT_REF_H4;  i++ )
         {
         vm_donnees_H4_ref[pa_caract->fenetre-1][j][i-1] = vm_donnees_H4_ref[pa_caract->fenetre-1][j][i];
         }
      vm_donnees_H4_ref[pa_caract->fenetre-1][j][CM_NB_MESURES_FT_REF_H4 -1] = vm_donnees_futures[pa_caract->fenetre-1][j][0];

      for (i=1;  i < XZISC_NB_LIG_MESURES;  i++ )
         {
         vm_donnees_futures[pa_caract->fenetre-1][j][i-1] = vm_donnees_futures[pa_caract->fenetre-1][j][i];
         }


      for (i=0;  i < XZISC_NB_LIG_MESURES;  i++ )
      {
	sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE,
			c_correspFenetre[pa_caract->fenetre-1], 
			(float) vm_donnees_futures[pa_caract->fenetre-1][j][i].Horodate, pa_caract->varcaract[j].no_station,
			pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, 
			(float) vm_donnees_futures[pa_caract->fenetre-1][j][i].Valeur);
	send_message_courbe(vl_msg, CM_BUFF_SOCK);
      }
      }
    	sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_FIN_DONNEES_COURBE, CM_COURBE_FT, c_correspFenetre[pa_caract->fenetre-1]);
	 send_message_courbe(vl_msg, CM_BUFF_SOCK);

   vm_nb_donnees_futures[pa_caract->fenetre-1] = vm_nb_donnees_futures[pa_caract->fenetre-1] - 1;

   /* Si toutes les donnees futures sont affichees, lire en base les suivantes */
   if ( vm_nb_donnees_futures[pa_caract->fenetre-1] == 0 )
      {
      MSCO_donnees_ft_ref_periodiques (pa_caract);
      }

   /*TODO*/
   /*raf graphe?  */
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Maj_Courbe_H4" );
   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Premiere mise a jour du graphe de reference H+4 dans une fenetre
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Maj_Courbe_H4_premiere(
          pa_caract)

          tm_caracteristique_courbe *pa_caract;


/*
* ARGUMENTS EN ENTREE :
*   va_dp : drawport
*   va_graph_H4 : objet graphe
*   pa_caract : caracteristiques des courbes du graphe
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Au lancement de la courbe
*
* FONCTION
*   Affiche les donnees lues en base
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_H4.c	1.7 10/03/07 : MSCO_Maj_Courbe_H4_premiere" ;

   char vl_nom_var[64];
   int t=0;  /* le temps, modulo la periode de mesures */
   int i=0, j=0, k=0, n=0;
  char vl_msg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Maj_Courbe_H4_premiere" );

   /*A Afficher les donnees initiales du graphe H+4 */
   t = ((int) vm_Horodate_Ft_ref_H4[pa_caract->fenetre-1] + CM_DUREE_SLOTIME_FT_REF) % CM_PERIODE_MESURES;
      for ( j=0; j < pa_caract->nb_courbes ; j++ )
      {
      /*A Mise a jour des vag des var des courbes */
   for ( i=0; i < XZISC_NB_LIG_MESURES ; i++ )
         {
         sprintf( vl_nom_var , "Ft_ref_H4_%d_%d" , j, pa_caract->fenetre );
         if ( (vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Validite == XDC_RADT_CORRECT) ||
              (vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Validite == XDC_RADT_RECONST) )
            {
            MSCO_Maj_vag_horodate( vl_nom_var , (float) vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Valeur,
					 (float) vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Horodate);
            }
        else
            {
            MSCO_Maj_vag_horodate( vl_nom_var , (float) (-1) ,
				 (float) vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Horodate);
            }
         }

	sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f",
                                   CMD_FLASH_DONNEES_COURBE,
                                   CM_COURBE_FT_EXTRPOLEE,
                                   c_correspFenetre[pa_caract->fenetre-1],
                                   vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Horodate,
                                   pa_caract->varcaract[j].no_station,
                                   pa_caract->varcaract[j].sens,
                                   pa_caract->varcaract[j].voie,
                                   pa_caract->varcaract[j].mesure,
                                   vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Valeur);
	if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
                   {
                           XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
                           return ;
                   }

      /*B SI LE SLOT N'EST PAS LE DERNIER, mettre a jour la mesure pour le suivant */
      if (i < CM_NB_SLOTIME_H4_FT_REF -1)
         {
	 t = (t + CM_DUREE_SLOTIME_FT_REF) % CM_PERIODE_MESURES;
	 if (t == 0)
	    {
	    k = k + 1;
	    if (k >= CM_NB_MESURES_FT_REF_H4)
	       {
	       k = CM_NB_MESURES_FT_REF_H4 -1;
               for ( j=0; j < pa_caract->nb_courbes; j++ )
                  {
                  for ( n=1; n < CM_NB_MESURES_FT_REF_H4; n++ )
                     {
		     vm_donnees_H4_ref[pa_caract->fenetre-1][j][n-1] = vm_donnees_H4_ref[pa_caract->fenetre-1][j][n];
                     }
		  vm_donnees_H4_ref[pa_caract->fenetre-1][j][k] = vm_donnees_futures[pa_caract->fenetre-1][j][0];
                  for ( n=1; n < vm_nb_donnees_futures[pa_caract->fenetre-1]; n++ )
                     {
		     vm_donnees_futures[pa_caract->fenetre-1][j][n-1] = vm_donnees_futures[pa_caract->fenetre-1][j][n];
                     }
                  }
		  vm_nb_donnees_futures[pa_caract->fenetre-1] = vm_nb_donnees_futures[pa_caract->fenetre-1] -1;
	       }
	    }
         }
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Maj_Courbe_H4_premiere" );

   return XDC_OK;
}

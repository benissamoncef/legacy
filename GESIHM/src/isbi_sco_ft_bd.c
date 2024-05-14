/*E*/
/*Fichier : $Id: isbi_sco_ft_bd.c,v 1.2 2012/05/29 10:09:28 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date :  $Date: 2012/05/29 10:09:28 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MSCO * FICHIER isyn_sco_ft_bd.c
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de traitement des courbes : Recuperation des donnees
*  a partir de la base de donnees (bd) pour les courbes F(t)
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	08 Nov 1994	: Creation
-----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sco_bd.h"
#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"


/* definitions de constantes */
#define CM_BUFF_SOCK 600

/* definitions de types locaux */
               
/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)isyn_sco_ft_bd.c	1.2 09/24/07 : isyn_sco_ft_bd" ;
static int vm_no_courbe[CM_NB_MAX_FENETRES]={0,0};
char vl_msg[CM_BUFF_SOCK];
tm_Tab_LignesDonnees vm_donneesbd[CM_MAX_COURBES_FT];

/* declaration de variables externes */
extern char c_correspFenetre[];
extern int 		vm_indice_HeurePrec;
extern XDY_Horodate vm_HorodateSecInit_H1;
extern int vm_TypeMachine;

XDY_Horodate vm_horodate_courbe;
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Recuperation des donnees pour les courbes a partir de la bd
*   pour les graphes F(t).
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_init_donnees_ft(
                    pa_caract)
                    
                    tm_caracteristique_courbe *pa_caract;

/*
* ARGUMENTS EN ENTREE :
*  pa_caract : Caracteristiques de la courbe.
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
*   Ouverture d'une courbe F(t) dans une fenetre
*
* FONCTION 
*   Effectue la requete pour les donnees de la courbe suivante du graphe,
*   arrete la recuperation des donnees apres la derniere courbe.
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_ft_bd.c	1.2 09/24/07 : MSCO_init_donnees_ft " ;
   T_BOOL vl_etat;
   XDY_Booleen vl_demande=XDC_OUI;
   int vl_no_fen=0;
   

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_init_donnees_ft" );
   
   /*A Test si la recuperation des donnees est finie ... */
   if ( vm_no_courbe[pa_caract->fenetre-1] == pa_caract->nb_courbes )
      {
      /*A On desarme la callback de recuperation des donnees*/
      if ( MSCO_arret_recuperation_FT(pa_caract->fenetre) == XDC_NOK ) 
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Desarmement de la Recuperation de donnees %s non effectuee. \n " ,XDG_IDONNEES_FT);
         return XDC_NOK;
         }	
      /*A Affichage des donnees recuperees sur le graph */
      MSCO_Afficher_les_donnees_recuperees(pa_caract);
      /*A Init des valeurs du graph H+1 */
      if ( MSCO_Init_Courbe_H1(pa_caract->fenetre) == XDC_NOK ) 
         {	
         XZST_03EcritureTrace( XZSTC_WARNING, "Probleme lors de l'init du graphe H+1");
         return XDC_NOK;
         }	

      MSCO00_Signaler_Fin_Init(pa_caract->fenetre-1, MSCO_Type_Courbe(pa_caract->fenetre));

      /*A On se branche sur le temps reel */
      if (pa_caract->tempsreel == XDC_OUI )
         {
         MSCO_Abt( pa_caract ,XZEAC_DEBUT_ABONNEMENT );
         MSCO_Lancement_raf_courbe(pa_caract->fenetre);
         }

      }
   else
      {
      /*A Demander les donnees pour la courbe (la variable) numero vm_no_courbe[fenetre] */
      if ( MSCO_requete_courbe (pa_caract, vm_no_courbe[pa_caract->fenetre-1]) != XDC_OK )
         return XDC_NOK;

      vm_no_courbe[pa_caract->fenetre-1] =  vm_no_courbe[pa_caract->fenetre-1]+1;
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_init_donnees_ft" );

   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Arret de la recuperation de donnees pour une fenetre
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_arret_recuperation_FT(
                    va_fenetre)

                    XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*  va_fenetre : Numero de la fenetre.
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
*   Terminaison ou abandon d'une requete de donnees RADT pour la fenetre
*
* FONCTION
*   Desactive toute eventuelle requete pour la fenetre.
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_ft_bd.c	1.2 09/24/07 : MSCO_arret_recuperation_FT " ;

   int vl_cr = XDC_OK;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_arret_recuperation_FT (%d)", va_fenetre );

   if ( MSCO_arret_recuperation (va_fenetre) != XDC_OK )
      {
      XZST_03EcritureTrace( XZSTC_WARNING, "Desarmement de la recuperation de donnees %s non effectuee " ,
                               XDG_IDONNEES_FT);
      vl_cr = XDC_NOK;
      }

   vm_no_courbe[va_fenetre-1] = 0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_arret_recuperation_FT" );

   return vl_cr;
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Recuperation des donnees pour les courbes a partir de la bd
   pour ft.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_Recup_donnees_FT(
        tm_caracteristique_courbe *pa_caract,
        T_INT2                     va_Existe,
        T_REAL8                    va_HorodateDebut,
        T_INT4                    *pa_valeur,
        T_INT2                    *pa_validite)

/*
* ARGUMENTS EN ENTREE :
*   pa_caract        : Caracteristiques de la courbe
*   va_Existe        : Indicateur de presence des donnees
*   va_HorodateDebut : Horodate de la premiere valeur
*   pa_valeur        : Liste des valeurs des donnees
*   pa_validite      : Liste des valeurs de validite des donnees.
* 
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Sur message de reception des donnees
*
* FONCTION 
*   Stockage et affichage des donnees recues
*
------------------------------------------------------*/
{//static char *version = "@(#)isyn_sco_ft_bd.c	1.2 09/24/07 : MSCO_Recup_donnees_FT " ;
   T_INT2 vl_Existe = va_Existe;
   T_REAL8 vl_HorodateDebut = va_HorodateDebut;
   T_INT4 *pl_valeur = pa_valeur;
   T_INT2 *pl_validite = pa_validite;
   int i=0,j=0;
   int vl_delta;
   int vt_existe = vl_Existe;
   int vt_valeur = *pl_valeur;
   int vt_validite = *pl_validite;

 
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Recup_donnees_FT existe=%d, valeur=%d, validite=%d", vt_existe, vt_valeur, vt_validite );
   
   j=vm_no_courbe[pa_caract->fenetre-1]-1;
   vl_delta=0;
   vl_delta = (int) ((vl_HorodateDebut-((tm_caracteristique_courbe *) pa_caract)->horodeb) / ( 60*6));
   /*A Si l'horodate renvoyee est diff que celle envoyee alors on ne fait plus de temps reel et on recale le tableau de donnees*/
   XZST_03EcritureTrace(   XZSTC_FONCTION, "DELTA %d // %lf // %lf",vl_delta,vl_HorodateDebut,((tm_caracteristique_courbe *) pa_caract)->horodeb);
   if ( vl_delta != 0 )
      {
      /************((tm_caracteristique_courbe *) pa_caract)->tempsreel = XDC_NON;
      MSCO_Afficher_typetemps(XDC_NON, ((tm_caracteristique_courbe *) pa_caract)->fenetre );  **************/
      if (vl_delta < 0 )
         {
         if ( abs(vl_delta) > XZISC_NB_LIG_MESURES )
            {
            for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
               {
               vm_donneesbd[j][i].Valeur = (float)XDC_RADT_CUMUL_INCONNU;
               vm_donneesbd[j][i].Validite = XDC_RADT_INCONNU;
               }
            vl_delta = XZISC_NB_LIG_MESURES+1;
            }
         else
            {
            XZST_03EcritureTrace(XZSTC_WARNING,"remplissage de %d a %d avec inconnu",XZISC_NB_LIG_MESURES-abs(vl_delta),XZISC_NB_LIG_MESURES );
            for (i=XZISC_NB_LIG_MESURES-abs(vl_delta); i < XZISC_NB_LIG_MESURES ; i++ )
               {
               vm_donneesbd[j][i].Valeur = (float)XDC_RADT_CUMUL_INCONNU;
               vm_donneesbd[j][i].Validite = XDC_RADT_INCONNU;
               }
            }
         /*A recomposition du tableau de donnees par variable de la courbe */
         /*A i les donnees sont inexistantes on rempli le tableau avec XDC_RADT_CUMUL_INCONNU et XDC_RADT_INCONNU */
         if ( vl_Existe == XDC_VRAI )
            {
            for (i=0; i < XZISC_NB_LIG_MESURES-abs(vl_delta) ; i++ )
               {
               vm_donneesbd[j][i].Valeur=(float)pl_valeur[i+abs(vl_delta)];
               vm_donneesbd[j][i].Validite=(XDY_Octet)pl_validite[i+abs(vl_delta)];
               }
            }
         else
            {
            for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
               {
               vm_donneesbd[j][i].Valeur=XDC_RADT_CUMUL_INCONNU;
               vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU ;
               }
            }
         }
      else
         {
         if ( vl_delta > XZISC_NB_LIG_MESURES )
            vl_delta = XZISC_NB_LIG_MESURES;
         for (i=0; i < vl_delta; i++ )
            {
            vm_donneesbd[j][i].Valeur=(float)XDC_RADT_CUMUL_INCONNU;
            vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU;
            }
         
         /*A recomposition du tableau de donnees par variable de la courbe */
         /*A i les donnees sont inexistantes on rempli le tableau avec XDC_RADT_CUMUL_INCONNU et XDC_RADT_INCONNU */
         if ( vl_Existe == XDC_VRAI )
            {
            for (i=vl_delta; i < XZISC_NB_LIG_MESURES ; i++ )
               {
               vm_donneesbd[j][i].Valeur=(float)pl_valeur[i-vl_delta];
               vm_donneesbd[j][i].Validite=(XDY_Octet)pl_validite[i-vl_delta];
               }
            }
         else
            {
            for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
               {
               vm_donneesbd[j][i].Valeur=(float)XDC_RADT_CUMUL_INCONNU;
               vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU ;
               }
            }
         } /*fin else du if  (vl_delta < 0 ) */
      }
      
   else    
      {
      /*A recomposition du tableau de donnees par variable de la courbe */
      /*A i les donnees sont inexistantes on rempli le tableau avec XDC_RADT_CUMUL_INCONNU et XDC_RADT_INCONNU */
	XZST_03EcritureTrace(   XZSTC_FONCTION, "EXISTE %d",vl_Existe);
      if ( vl_Existe == XDC_VRAI )
         {
         for (i=vl_delta; i < XZISC_NB_LIG_MESURES ; i++ )
            {
            vm_donneesbd[j][i].Valeur=(float)pl_valeur[i-vl_delta];
            vm_donneesbd[j][i].Validite=(XDY_Octet)pl_validite[i-vl_delta];
            }
         }
      else
         {
         for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
            {
            vm_donneesbd[j][i].Valeur=(float)XDC_RADT_CUMUL_INCONNU;
            vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU ;
            }
         }
      }

   MSCO_init_donnees_ft( (tm_caracteristique_courbe *) pa_caract );
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Recup_donnees_FT" );
   return ;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Afficher les donnees recuperees de la base et bufferisees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Afficher_les_donnees_recuperees(pa_caract)
                    
                    tm_caracteristique_courbe *pa_caract;

 

/*
* ARGUMENTS EN ENTREE :
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
*   Creation de la table de hash
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_ft_bd.c	1.2 09/24/07 : MSCO_Afficher_les_donnees_recuperees " ;
 int i=0;
 int j=0;
 int k=0;
  char vl_nom_var[256];
  float vl_res=0.0;
   float *pl_valeur;
   XZSMT_Horodate vl_horodate;
   XDY_Horodate vl_horodatesec=0;
   char *pl_valeur_texte;
   float vl_valeur;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :MSCO_Afficher_les_donnees_recuperees " );

   for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
      {
	   vl_horodatesec = pa_caract->horodeb + i*60*6;


	   for ( j=0; j < CM_MAX_COURBES_FT ; j++ )
         {
		   if (pa_caract->varcaract[j].no_station != 0){
			 sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station ,
												   pa_caract->varcaract[j].sens ,
												   pa_caract->varcaract[j].voie ,
												   pa_caract->varcaract[j].mesure );

			 if ( (vm_donneesbd[j][i].Validite == XDC_RADT_CORRECT) ||
				  (vm_donneesbd[j][i].Validite == XDC_RADT_RECONST) )
				{
				MSCO_Maj_vag( vl_nom_var , (float)vm_donneesbd[j][i].Valeur );
				vl_valeur = vm_donneesbd[j][i].Valeur;
				}
			else
				{
				MSCO_Maj_vag( vl_nom_var , (float)-1 );
				vl_valeur = -1;
				}

	// envoi donnée
			sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, c_correspFenetre[pa_caract->fenetre-1], vl_horodatesec, pa_caract->varcaract[j].no_station, pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, vl_valeur);
			if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
				return ;
			}

			 if ( i >= XZISC_NB_LIG_MESURES-CM_UNE_HEURE_DE_DONNEES )
				{
				MSCO_Calcul_Donnees_H1( (float)vm_donneesbd[j][i].Valeur ,
										(float)vm_donneesbd[j][i+CM_54_MINUTES_DE_DONNEES].Valeur ,
										&vl_res );
				vm_donneesbd[j][i+CM_UNE_HEURE_DE_DONNEES].Valeur=(float)vl_res;

/*
				// envoi donnée
				sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE, c_correspFenetre[pa_caract->fenetre-1], vl_horodatesec+(60*60), pa_caract->varcaract[j].no_station, pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, vl_res);
				if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
				{
					XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
					return ;
				}
*/
				}
			 }
         }
      MSCO03_raf_courbe_temps_init(pa_caract->fenetre);
      }
   
   vm_indice_HeurePrec=0;

   vm_horodate_courbe = vl_horodatesec+360;

	/* Init de la legende aux dernieres valeurs */
      
      XZSM_11ConversionHorodate((trunc(vm_HorodateSecInit_H1/(6*60)))*(6*60) , &vl_horodate);
      vl_horodate[16] = 0; /* on elimine les decondes */
            

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :MSCO_Afficher_les_donnees_recuperees  " );
   return ;
}

/*E*/
/*Fichier : $Id: isbi_sco_ft_ref_bd.c,v 1.2 2012/05/29 10:09:28 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 10:09:28 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MSCO * FICHIER isbi_sco_ft_ref_bd.c
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de traitement des courbes : Recuperation des donnees
*  a partir de la base de donnees pour les courbes avec reference
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		30/08/2007 : Creation 1.1
-----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"

/* definitions de constantes */

#define CM_6HEURES  (3600 * 6)
#define CM_BUFF_SOCK 600
/* Valeurs definissant la nature de la requete en cours */
#define CM_RQ_DONNEES_AUCUNE             0
#define CM_RQ_DONNEES_3_DERNIERES_HEURES 1   /* ... des courbes temps reel */
#define CM_RQ_DONNEES_REF_INITIALES      2   /* premieres donnees des courbes de reference */
#define CM_RQ_DONNEES_REF_INITIALES_H4   3   /* pour completer les courbes H+4 */
#define CM_RQ_DONNEES_REF_SUIVANTES      4   /* apres l'init., toutes les 6 heures */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)isbi_sco_ft_ref_bd.c	1.11 10/03/07 : isbi_sco_ft_ref_bd" ;

/* Donnees de la requete en cours */
static int vm_no_courbe_ft_ref[CM_NB_MAX_FENETRES]={0,0};
static int vm_type_requete[CM_NB_MAX_FENETRES]={ CM_RQ_DONNEES_AUCUNE, CM_RQ_DONNEES_AUCUNE };
static int vm_nb_rqt_periodiques[CM_NB_MAX_FENETRES] = {1,1};

extern XDY_Horodate vm_Horodate_Ft_ref_Init[CM_NB_MAX_FENETRES];
extern XDY_Horodate vm_Horodate_Ft_Reference[CM_NB_MAX_FENETRES];
extern XDY_Horodate vm_Horodate_Ft_ref_Appel[CM_NB_MAX_FENETRES];

static mon_XZIST_LigneDonnees vm_donnees_ft_H_3[CM_NB_MAX_FENETRES][CM_MAX_COURBES_FT_REF][CM_NB_MESURES_FT_REF+1];
mon_XZIST_LigneDonnees vm_donnees_H4_ref[CM_NB_MAX_FENETRES][CM_MAX_COURBES_FT_REF][XZISC_NB_LIG_MESURES];
mon_XZIST_LigneDonnees vm_donnees_futures[CM_NB_MAX_FENETRES][CM_MAX_COURBES_FT_REF][XZISC_NB_LIG_MESURES];
int vm_nb_donnees_futures[CM_NB_MAX_FENETRES];


/* definition de variables externes */
extern char c_correspFenetre[];


/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Requete initiale de recuperation dans la base de donnees
*  de toutes les donnees necessaires a l'affichage initial des courbes
*  pour les graphes F(t) avec reference.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_init_donnees_ft_ref(
                    pa_caract)

                    tm_caracteristique_courbe *pa_caract;

/*
* ARGUMENTS EN ENTREE :
*  pa_caract : Caracteristiques de la courbe.
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
*   Ouverture d'une courbe F(t) avec reference dans une fenetre
*
* FONCTION
*   Effectue la requete pour les premieres donnees
*   de la premieres courbe du graphe.
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_bd.c	1.11 10/03/07 : MSCO_init_donnees_ft_ref " ;
 XZSMT_Horodate vl_horodate;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_init_donnees_ft_ref" );

   /*A Verifier qu'aucune requete n'est en cours pour cette fenetre */
   if ( vm_type_requete[pa_caract->fenetre-1] != CM_RQ_DONNEES_AUCUNE )
      {
      XZST_03EcritureTrace( XZSTC_WARNING, "Requete de donnees de reference deja en cours dans la fenetre %d",
                               pa_caract->fenetre);
      return XDC_NOK;
      }

   /*A Invoquer la requete pour les premieres donnees de la premiere courbe */
   vm_no_courbe_ft_ref[pa_caract->fenetre-1] = 0;
   vm_type_requete[pa_caract->fenetre-1] = CM_RQ_DONNEES_3_DERNIERES_HEURES;

   pa_caract->horodeb = vm_Horodate_Ft_ref_Init[pa_caract->fenetre-1];
   XZSM_11ConversionHorodate( pa_caract->horodeb,&vl_horodate);
   /* commencer a la mesure du (ou precedant le) prochain slot (premier slot de la courbe) */
   pa_caract->horodeb = pa_caract->horodeb + CM_DUREE_SLOTIME_FT_REF - (CM_PERIODE_MESURES - 1);
   XZSM_11ConversionHorodate( pa_caract->horodeb,&vl_horodate);

   XZST_03EcritureTrace( XZSTC_DEBUG1, "Requete %d pour courbe %d, fenetre %d - date=%f",
                            vm_type_requete[pa_caract->fenetre-1], vm_no_courbe_ft_ref[pa_caract->fenetre-1],
                            pa_caract->fenetre, pa_caract->horodeb );
   if ( MSCO_requete_courbe (pa_caract, vm_no_courbe_ft_ref[pa_caract->fenetre-1]) != XDC_OK )
      return XDC_NOK;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_init_donnees_ft_ref" );

   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Requete periodique de recuperation dans la base de donnees
*  des donnees de reference pour la periode suivante.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_donnees_ft_ref_periodiques(
                    pa_caract)

                    tm_caracteristique_courbe *pa_caract;

/*
* ARGUMENTS EN ENTREE :
*  pa_caract : Caracteristiques de la courbe.
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
*   En cours de fonctionnement,
*   apres consommation de toutes les donnees de reference.
*
* FONCTION
*   Effectue la requete pour les donnees
*   de la premieres courbe de reference du graphe.
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_bd.c	1.11 10/03/07 : MSCO_donnees_ft_ref_periodiques " ;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_donnees_ft_ref_periodiques" );

   /*A Verifier qu'aucune requete n'est en cours pour cette fenetre */
   if ( vm_type_requete[pa_caract->fenetre-1] != CM_RQ_DONNEES_AUCUNE )
      {
      XZST_03EcritureTrace( XZSTC_WARNING, "Requete de donnees de reference deja en cours dans la fenetre %d",
                               pa_caract->fenetre);
      return XDC_NOK;
      }

   /*A Invoquer la requete pour les donnees de la premiere courbe */
   vm_no_courbe_ft_ref[pa_caract->fenetre-1] = 0;
   vm_type_requete[pa_caract->fenetre-1] = CM_RQ_DONNEES_REF_SUIVANTES;

   vm_nb_rqt_periodiques[pa_caract->fenetre-1] = vm_nb_rqt_periodiques[pa_caract->fenetre-1] +1;
   pa_caract->horodeb = vm_Horodate_Ft_Reference[pa_caract->fenetre-1] +
                           (vm_nb_rqt_periodiques[pa_caract->fenetre-1] * CM_6HEURES);
   /* commencer a la mesure du (ou precedant le) slot suivant celui a cette date */
   pa_caract->horodeb = pa_caract->horodeb + CM_DUREE_SLOTIME_FT_REF - (CM_PERIODE_MESURES - 1);

   XZST_03EcritureTrace( XZSTC_DEBUG1, "Requete %d pour courbe %d, fenetre %d - date=%f",
                            vm_type_requete[pa_caract->fenetre-1], vm_no_courbe_ft_ref[pa_caract->fenetre-1],
                            pa_caract->fenetre, pa_caract->horodeb );
   if ( MSCO_requete_courbe (pa_caract, vm_no_courbe_ft_ref[pa_caract->fenetre-1]) != XDC_OK )
      return XDC_NOK;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_donnees_ft_ref_periodiques" );

   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Requete de recuperation dans la base de donnees
*  des donnees suivantes pour l'affichage des courbes
*  des graphes F(t) avec reference.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static MSCO_Donnees_FT_Ref_Suivantes(
                    pa_caract)

                    tm_caracteristique_courbe *pa_caract;

/*
* ARGUMENTS EN ENTREE :
*  pa_caract : Caracteristiques de la courbe.
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
*   Reception des donnees de la requete en cours
*
* FONCTION
*   Effectue la requete pour les donnees suivantes,
*   arrete la recuperation lorsque toutes les donnees sont obtenues.
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_bd.c	1.11 10/03/07 : MSCO_Donnees_FT_Ref_Suivantes " ;

   int vl_cr = XDC_OK;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Donnees_FT_Ref_Suivantes" );

   /*A Selon la nature de la requete qui vient de se terminer */
   switch ( vm_type_requete[pa_caract->fenetre-1] )
      {
      case CM_RQ_DONNEES_3_DERNIERES_HEURES :
	 /* Demander les premieres donnees de reference pour la meme courbe */
         vm_type_requete[pa_caract->fenetre-1] = CM_RQ_DONNEES_REF_INITIALES;
	 break;

      case CM_RQ_DONNEES_REF_INITIALES :
	 /* Demander les donnees de reference pour les heures qui suivent pour la meme courbe */
         vm_type_requete[pa_caract->fenetre-1] = CM_RQ_DONNEES_REF_INITIALES_H4;
	 break;

      case CM_RQ_DONNEES_REF_INITIALES_H4 :
	 /* Toutes les donnees sont lues pour cette courbe, passer a la suivante */
         vm_no_courbe_ft_ref[pa_caract->fenetre-1] = vm_no_courbe_ft_ref[pa_caract->fenetre-1] + 1;
         vm_type_requete[pa_caract->fenetre-1] = CM_RQ_DONNEES_3_DERNIERES_HEURES;
	 break;

      case CM_RQ_DONNEES_REF_SUIVANTES :
	 /* Passer a la courbe suivante ou stopper la recuperation si c'est la derniere */
         vm_no_courbe_ft_ref[pa_caract->fenetre-1] = vm_no_courbe_ft_ref[pa_caract->fenetre-1] + 1;
	 break;

      default :
         /* Aucune requete ??? */
         vm_no_courbe_ft_ref[pa_caract->fenetre-1] = pa_caract->nb_courbes;
	 break;
      }

   /*A Si les donnees de toutes les courbes ont ete lues, */
   if ( vm_no_courbe_ft_ref[pa_caract->fenetre-1] >= pa_caract->nb_courbes )
      {
      /*A desactiver la recuperation des donnees en base */
      if ( MSCO_arret_recuperation_FT_Ref(pa_caract->fenetre) == XDC_NOK )
         {
         vl_cr = XDC_NOK;
         }
      }
   else
      {
      /*A sinon invoquer la requete pour les donnees suivantes */
      switch ( vm_type_requete[pa_caract->fenetre-1] )
         {
         case CM_RQ_DONNEES_3_DERNIERES_HEURES :
            pa_caract->horodeb = vm_Horodate_Ft_ref_Init[pa_caract->fenetre-1];
	    break;
         case CM_RQ_DONNEES_REF_INITIALES :
            pa_caract->horodeb = vm_Horodate_Ft_Reference[pa_caract->fenetre-1];
	    break;
         case CM_RQ_DONNEES_REF_INITIALES_H4 :
            pa_caract->horodeb = vm_Horodate_Ft_Reference[pa_caract->fenetre-1] + CM_6HEURES;
	    break;
         case CM_RQ_DONNEES_REF_SUIVANTES :
            pa_caract->horodeb = vm_Horodate_Ft_Reference[pa_caract->fenetre-1] +
                                    (vm_nb_rqt_periodiques[pa_caract->fenetre-1] * CM_6HEURES);
	    break;
         }
         /* commencer a la mesure du (ou precedant le) slot suivant celui a cette date */
         pa_caract->horodeb = pa_caract->horodeb + CM_DUREE_SLOTIME_FT_REF - (CM_PERIODE_MESURES - 1);

      XZST_03EcritureTrace( XZSTC_DEBUG1, "Requete %d pour courbe %d, fenetre %d - date=%f",
                               vm_type_requete[pa_caract->fenetre-1], vm_no_courbe_ft_ref[pa_caract->fenetre-1],
                               pa_caract->fenetre, pa_caract->horodeb );
      if ( MSCO_requete_courbe (pa_caract, vm_no_courbe_ft_ref[pa_caract->fenetre-1]) != XDC_OK )
         {
         vl_cr = XDC_NOK;
         }
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_Donnees_FT_Ref_Suivantes" );

   return vl_cr;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Arret de la recuperation de donnees pour une fenetre
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_arret_recuperation_FT_Ref(
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
{static char *version = "@(#)isbi_sco_ft_ref_bd.c	1.11 10/03/07 : MSCO_arret_recuperation_FT_Ref " ;

   int vl_cr = XDC_OK;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_arret_recuperation_FT_Ref" );

   if ( MSCO_arret_recuperation (va_fenetre) != XDC_OK )
      {
      XZST_03EcritureTrace( XZSTC_WARNING, "Desarmement de la recuperation de donnees %s non effectuee " ,
                               XDG_IDONNEES_FT);
      vl_cr = XDC_NOK;
      }

   vm_no_courbe_ft_ref[va_fenetre-1] = 0;
   vm_type_requete[va_fenetre-1] = CM_RQ_DONNEES_AUCUNE;
   vm_nb_rqt_periodiques[va_fenetre-1] = 1;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_arret_recuperation_FT_Ref" );

   return vl_cr;
}





/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*   Recuperation des donnees pour les courbes a partir de la base de donnees
*  pour les graphes F(t) avec reference.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_Recup_donnees_FT_Ref(
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
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_bd.c	1.11 10/03/07 : MSCO_Recup_donnees_FT_Ref " ;

   XDY_Horodate vl_HorodateDebut = va_HorodateDebut;
   XDY_Booleen vl_fin_init = XDC_NON;
   mon_XZIST_LigneDonnees vl_donnees_recues[XZISC_NB_LIG_MESURES];
   int i=0, j=0;
   int vl_delta=0;
   XZSMT_Horodate vl_horodate;
   XDY_Horodate vl_horo;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : JM4 MSCO_Recup_donnees_FT_Ref" );

   /*B Initialiser les donnees recues avec des valeurs par defaut ('Inconnu') */
   j = vm_no_courbe_ft_ref[pa_caract->fenetre-1];
   for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
      {
      XZST_03EcritureTrace(   XZSTC_FONCTION, " JM2 i = %d",i);
      vl_donnees_recues[i].Valeur=(float)XDC_RADT_CUMUL_INCONNU;
      vl_donnees_recues[i].Horodate = pa_caract->horodeb + i * 6 * 60;
       vl_donnees_recues[i].Validite=(XDY_Octet)XDC_RADT_INCONNU;
     	XZSM_11ConversionHorodate( vl_donnees_recues[i].Horodate  , &vl_horodate);
      }

   j = vm_no_courbe_ft_ref[pa_caract->fenetre-1];
   /*A Si l'horodate recue est differente de celle envoyee alors completer les donnees manquantes */
   if ( va_HorodateDebut < pa_caract->horodeb )
      {
      vl_delta = (int) ((pa_caract->horodeb + (6*60-1) - va_HorodateDebut) / ( 60*6));
      if ( vl_delta < XZISC_NB_LIG_MESURES )
         XZST_03EcritureTrace(XZSTC_WARNING, "Remplissage de %d a %d avec inconnu",
                                 XZISC_NB_LIG_MESURES-vl_delta ,XZISC_NB_LIG_MESURES );

      /*A Recuperation des donnees existantes */
      if ( va_Existe == XDC_VRAI )
         {
         for (i=0; i < XZISC_NB_LIG_MESURES-vl_delta ; i++ )
            {
            vl_donnees_recues[i].Valeur=(float)pa_valeur[i+vl_delta];
            vl_donnees_recues[i].Validite=(XDY_Octet)pa_validite[i+vl_delta];
            }
         }
      }
   else
      {
      vl_delta = (int) ((va_HorodateDebut - pa_caract->horodeb) / ( 60*6));
      /*A Recuperation des donnees existantes */
      if ( va_Existe == XDC_VRAI )
         {
         for (i=vl_delta; i < XZISC_NB_LIG_MESURES ; i++ )
            {
            vl_donnees_recues[i].Valeur=(float)pa_valeur[i-vl_delta];
            vl_donnees_recues[i].Validite=(XDY_Octet)pa_validite[i-vl_delta];
            }
         }
      }


   /* Mettre a jour les donnees de la courbe adequate */
   switch ( vm_type_requete[pa_caract->fenetre-1] )
      {
      case CM_RQ_DONNEES_3_DERNIERES_HEURES :
         /*B Recuperer trois heures de donnees valides, plus une eventuellement */
         for (i=0; i <= CM_NB_MESURES_FT_REF; i++ )
            {
            vm_donnees_ft_H_3[pa_caract->fenetre-1][j][i].Valeur = vl_donnees_recues[i].Valeur;
            vm_donnees_ft_H_3[pa_caract->fenetre-1][j][i].Validite = vl_donnees_recues[i].Validite;
            vm_donnees_ft_H_3[pa_caract->fenetre-1][j][i].Horodate = vl_donnees_recues[i].Horodate;
	    XZST_03EcritureTrace(   XZSTC_FONCTION,"vm_donnees_ft_H_3 %d %lf",i,vl_donnees_recues[i].Horodate);
            }
	 break;

      case CM_RQ_DONNEES_REF_INITIALES :
         /*B Recuperer trois heures de reference, plus trois heures H+4 */
         for (i=0; i < XZISC_NB_LIG_MESURES; i++ )
            {
	    vm_donnees_H4_ref[pa_caract->fenetre-1][j][i].Horodate = vm_donnees_ft_H_3[pa_caract->fenetre-1][j][0].Horodate + i * 6 * 60;
            vm_donnees_H4_ref[pa_caract->fenetre-1][j][i].Valeur = vl_donnees_recues[i].Valeur;
            vm_donnees_H4_ref[pa_caract->fenetre-1][j][i].Validite = vl_donnees_recues[i].Validite;
            /*vm_donnees_H4_ref[pa_caract->fenetre-1][j][i].Horodate = vm_donnees_ft_H_3[pa_caract->fenetre-1][j][i].Horodate; vl_donnees_recues[i].Horodate;*/
	    XZST_03EcritureTrace(   XZSTC_FONCTION,"vm_donnees_H4_ref %d %lf %d",i,vm_donnees_H4_ref[pa_caract->fenetre-1][j][i].Horodate, j);
            }
	 break;

      case CM_RQ_DONNEES_REF_INITIALES_H4 :
      case CM_RQ_DONNEES_REF_SUIVANTES :
         /*B Recuperer six heures de donnees de reference futures */
         for (i=0; i < XZISC_NB_LIG_MESURES; i++ )
            {
            vm_donnees_futures[pa_caract->fenetre-1][j][i].Valeur = vl_donnees_recues[i].Valeur;
            vm_donnees_futures[pa_caract->fenetre-1][j][i].Validite = vl_donnees_recues[i].Validite;
            vm_donnees_futures[pa_caract->fenetre-1][j][i].Horodate = vl_donnees_recues[i].Horodate;
	    XZST_03EcritureTrace(   XZSTC_FONCTION,"vm_donnees_futures %d %lf",i,vl_donnees_recues[i].Horodate);
            }
	 break;

      default : /* donnees inattendues */
	 break;
      }


   XZST_03EcritureTrace( XZSTC_WARNING, "JM3 %d // %d", vm_no_courbe_ft_ref[pa_caract->fenetre-1], pa_caract->nb_courbes -2);

   /*B Lorsque les donnees de toutes les courbes ont ete recuperees, */
   if ( vm_no_courbe_ft_ref[pa_caract->fenetre-1] == pa_caract->nb_courbes -2 )
      {
      /*B memoriser le compte de donnees et marquer la fin de l'initialisation */
      vm_nb_donnees_futures[pa_caract->fenetre-1] = XZISC_NB_LIG_MESURES;

      if ( vm_type_requete[pa_caract->fenetre-1] == CM_RQ_DONNEES_REF_INITIALES_H4 )
         vl_fin_init = XDC_OUI;
      }


   /*A Demander les donnees suivantes ou arreter la recuperation */
   MSCO_Donnees_FT_Ref_Suivantes(pa_caract);


   /*A Apres avoir recupere toutes les donnees necessaires a l'initialisation, */
   if ( vl_fin_init == XDC_OUI )
      {
      /*A afficher les donnees recuperees sur le graphe */
      MSCO_Afficher_donnees_FT_Ref_recuperees (pa_caract);
      /*A et les donnees de reference H+4 sur leur graphe */
     /* if ( mSCO_Init_Courbe_FT_Ref_H4(pa_caract->fenetre) == XDC_NOK )
         {
         XZST_03EcritureTrace( XZSTC_WARNING, "Probleme lors de l'init du graphe reference H+4");
         }
*/
      /*A puis activer le fonctionnement temps reel */
      MSCO_Abt( pa_caract ,XZEAC_DEBUT_ABONNEMENT );
      MSCO_Lancement_raf_courbe(pa_caract->fenetre);

      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Recup_donnees_FT_Ref" );

   return ;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Afficher les donnees recuperees de la base et bufferisees.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Afficher_donnees_FT_Ref_recuperees(pa_caract)

                    tm_caracteristique_courbe *pa_caract;



/*
* ARGUMENTS EN ENTREE :
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
*   Lorsque toutes les donnees sont recuperees
*
* FONCTION
*   Mise a jour des variables des courbes
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref_bd.c	1.11 10/03/07 : MSCO_Afficher_donnees_FT_Ref_recuperees " ;

   char vl_nom_var[64];
   char vl_msg[256];
   float *pl_valeur;
   XZSMT_Horodate vl_horodate;
   char pl_valeur_texte[20];
   int t=0;  /* le temps, modulo la periode de mesures */
   int i=0, j=0, k=0, i_futures=0;
   int vl_derniere_mesure=0;
   int l = 0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :MSCO_Afficher_donnees_FT_Ref_recuperees " );

   /*MSCO_reset_graphe_ft_ref( pa_caract->fenetre );*/

   t = ((int) vm_Horodate_Ft_ref_Init[pa_caract->fenetre-1] + CM_DUREE_SLOTIME_FT_REF) % CM_PERIODE_MESURES;
      /* pour chaque courbe demandee*/
      /* courbe 1 : j= 0 => donnees TR*/
      /*            j = 1 => donnees REF*/
      /*Ã courbe 2  j= 2 => donnees TR*/
      /*           j= 3 => donneees REF*/

      for ( j=0; j < pa_caract->nb_courbes ; j++ )
      {
   for (k=0; k < XZISC_NB_LIG_MESURES ; k++ )
         {
	 vl_derniere_mesure = k;

         /*A afficher les valeurs de la courbe temps reel */
         sprintf( vl_nom_var , "%d_%d_%d_%d" , pa_caract->varcaract[j].no_station ,
                                               pa_caract->varcaract[j].sens ,
                                               pa_caract->varcaract[j].voie ,
                                               pa_caract->varcaract[j].mesure );

	 XZST_03EcritureTrace(   XZSTC_FONCTION, " JM j = %d ; k = %d", j,k);
	 if (j%2==0) {
	 if ( vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Horodate > 0) {
         if ( (vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Validite == XDC_RADT_CORRECT) ||
              (vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Validite == XDC_RADT_RECONST) )
            {
	    XZSM_11ConversionHorodate(vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Horodate , &vl_horodate);
	    sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, 
					c_correspFenetre[pa_caract->fenetre-1], 
					(float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Horodate, 
					pa_caract->varcaract[j].no_station, 
					pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, 
					(float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Valeur);

            //MSCO_Maj_vag( vl_nom_var , (float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Valeur );
            }
        else if ( k == CM_NB_SLOTIME_FT_REF -1 )
            {
	    /* au cas ou la derniere mesure n'est pas encore arrivee, completer pour continuite */
            vl_derniere_mesure = k-1;
         //   MSCO_Maj_vag( vl_nom_var , (float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k-1].Valeur );
	    XZSM_11ConversionHorodate(vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k-1].Horodate , &vl_horodate);
	    sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, 
					c_correspFenetre[pa_caract->fenetre-1], 
					(float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Horodate, 
					pa_caract->varcaract[j].no_station, 
					pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, 
					(float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k-1].Valeur);
            }
        else
            {
          //  MSCO_Maj_vag( vl_nom_var , (float) (-1) );
	    XZSM_11ConversionHorodate(vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Horodate , &vl_horodate);
	    sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, 
					c_correspFenetre[pa_caract->fenetre-1], 
					(float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Horodate, 
					pa_caract->varcaract[j].no_station, 
					pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, 
					(float) vm_donnees_ft_H_3[pa_caract->fenetre-1][j][k].Valeur);
            }
         /*MSCO_Maj_vag( vl_nom_var , (float) (40 + 3*k) ); /* pour test */
	    send_message_courbe(vl_msg, CM_BUFF_SOCK);
	}
	}
	 else {
	 l = j-1;
         /*A puis celles de la courbe de reference correspondante */
         sprintf( vl_nom_var , "Ft_ref_%d_%d" , j, pa_caract->fenetre );
         if ( (vm_donnees_H4_ref[pa_caract->fenetre-1][l][k].Validite == XDC_RADT_CORRECT) ||
              (vm_donnees_H4_ref[pa_caract->fenetre-1][l][k].Validite == XDC_RADT_RECONST) )
            {
           // MSCO_Maj_vag( vl_nom_var , (float) vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Valeur );
	    XZSM_11ConversionHorodate(vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Horodate , &vl_horodate);
	    sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE, 
					c_correspFenetre[pa_caract->fenetre-1], 
					(float) vm_donnees_H4_ref[pa_caract->fenetre-1][l][k].Horodate, 
					pa_caract->varcaract[j].no_station, 
					pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, 
					(float) vm_donnees_H4_ref[pa_caract->fenetre-1][l][k].Valeur);
            }
         else
            {
            //MSCO_Maj_vag( vl_nom_var , (float) (-1) );
	    XZSM_11ConversionHorodate(vm_donnees_H4_ref[pa_caract->fenetre-1][j][k].Horodate , &vl_horodate);
	    sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE, 
					c_correspFenetre[pa_caract->fenetre-1], 
					(float) vm_donnees_H4_ref[pa_caract->fenetre-1][l][k].Horodate, 
					pa_caract->varcaract[j].no_station, 
					pa_caract->varcaract[j].sens, pa_caract->varcaract[j].voie, pa_caract->varcaract[j].mesure, 
					(float) vm_donnees_H4_ref[pa_caract->fenetre-1][l][k].Valeur);
            }
	    send_message_courbe(vl_msg, CM_BUFF_SOCK);
            }
	}
      /*MSCO_raf_courbe_ft_ref_init(pa_caract->fenetre);*/

      /*B Pour le slot suivant passer a la date suivante, et a la mesure suivante en debut de periode */
      t = (t + CM_DUREE_SLOTIME_FT_REF) % CM_PERIODE_MESURES;
      XZST_03EcritureTrace(   XZSTC_FONCTION, "JM t = %d",t);
      /*if (t == 0)
	 {
	 k = k + 1;
	 }*/
         }
      sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_FIN_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE, c_correspFenetre[pa_caract->fenetre-1]);
      send_message_courbe(vl_msg, CM_BUFF_SOCK);
      sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_FIN_INIT_COURBE, CM_COURBE_FT_REF, c_correspFenetre[pa_caract->fenetre-1]);
      send_message_courbe(vl_msg, CM_BUFF_SOCK);

   /*MSCO_raf_graphe_ft_ref( pa_caract->fenetre );*/

   /*A Afficher l'heure */
   XZSM_11ConversionHorodate((trunc(vm_Horodate_Ft_ref_Appel[pa_caract->fenetre-1]/(6*60)))*(6*60) , &vl_horodate);
   vl_horodate[16] = 0; /* supprimer les secondes */

   strcpy( pl_valeur_texte , vl_horodate);

   k=1;
   /*B Recadrer les mesures H+4 restantes, completer avec les donnees futures */
   for ( j=0; j < pa_caract->nb_courbes; j++ )
      {
      for ( i=k; i < XZISC_NB_LIG_MESURES; i++ )
         {
         vm_donnees_H4_ref[pa_caract->fenetre-1][j][i-k] = vm_donnees_H4_ref[pa_caract->fenetre-1][j][i];
         }

      /* Completer les donnees du graphe H+4 au nombre de mesures voulu */
      for ( i=XZISC_NB_LIG_MESURES-k, i_futures=0; i < CM_NB_MESURES_FT_REF_H4; i++, i_futures++ )
         {
         vm_donnees_H4_ref[pa_caract->fenetre-1][j][i] = vm_donnees_futures[pa_caract->fenetre-1][j][i_futures];
         }

      /* Recadrer les donnees futures restantes et memoriser leur quantite */
      for ( i=i_futures; i < XZISC_NB_LIG_MESURES; i++ )
         {
         vm_donnees_futures[pa_caract->fenetre-1][j][i-i_futures] = vm_donnees_futures[pa_caract->fenetre-1][j][i];
         }
      }
   vm_nb_donnees_futures[pa_caract->fenetre-1] = XZISC_NB_LIG_MESURES - i_futures;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :MSCO_Afficher_donnees_FT_Ref_recuperees  " );
   return ;
}

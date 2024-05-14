/*E*/
/*Fichier : $Id: xzao352.c,v 1.3 2018/02/09 15:18:59 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/02/09 15:18:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao352.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	02/10/08	: creation 
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.2
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao352.c,v 1.3 2018/02/09 15:18:59 devgfi Exp $ : xzao352" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des zones d'influence TUN et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao352_Lire_Config_Zon_TUN ( 	FILE  		*pa_Fd,
					XDY_FonctionInt pa_FonctionUtil,
					XDY_Entier   	*pa_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* FILE			pa_Fd
* XDY_FonctionInt	pa_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*pa_Resultat:OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*    Appelle la fonction utilisateur avec les arguments:
*	contenu de la ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
   char			vl_autoroute_tun[10];
   XDY_PR		vl_pr_tun;
   XDY_Sens		vl_sens_tun;
   char			vl_autoroute_zone[10];
   XDY_Sens		vl_sens_zone;
   XDY_PR		vl_debut_zone,vl_fin_zone;
   XDY_Booleen		vl_inhibition_zone;
   XDY_Octet		vl_type_zone ;
   char			vl_groupe_tunnel[5];
   XDY_PR		vl_PRPSTT;

   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao352: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È pa_Fd
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier pa_Fd 
       */
    
       vl_num_ligne++; 
       
       if (vl_Resultat_out != XZAO_EOF )
       { 
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */
          
          *pa_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != XDC_OK) 

          {
             /*A
             ** Extraction des differents champs 
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie Autoroute TUN non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_autoroute_tun, vl_Texte_out);
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %s",vl_autoroute_tun);
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie sens TUN non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_sens_tun = atoi ( vl_Texte_out ); 
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_sens_tun);
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie PR TUN non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_pr_tun= atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_pr_tun);
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie autoroute zone  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_autoroute_zone, vl_Texte_out);
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %s",vl_autoroute_zone);
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR , vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie sens zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     vl_sens_zone = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_sens_zone);
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie pr debut zone non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     vl_debut_zone = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_debut_zone);
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie PR fin zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_fin_zone = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_fin_zone);
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie type zone non valide(%s)",vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_type_zone = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_type_zone);
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie groupe tunnel  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy(vl_groupe_tunnel, vl_Texte_out ); 
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %s",vl_groupe_tunnel);
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie PR PSTT TUN non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_PRPSTT = atoi ( vl_Texte_out ); 
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_PRPSTT);
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : Saisie Inhibition non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_inhibition_zone = atoi ( vl_Texte_out );
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : %d",vl_inhibition_zone);
             
	     /*
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 :avant fct util");
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao352: %s / %d / %d / %s ",
							vl_autoroute_tun,
							vl_sens_tun,
							vl_pr_tun,
							vl_autoroute_zone);
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao352: %d / %d / %d / %d /%s / %d / %d",
					vl_sens_zone,
					vl_debut_zone,
					vl_fin_zone ,vl_type_zone,vl_groupe_tunnel,
					vl_PRPSTT,vl_inhibition_zone);

                if ( (vl_retcode = (*pa_FonctionUtil)  ( vl_autoroute_tun
							  ,vl_sens_tun
							  ,vl_pr_tun
							  ,vl_autoroute_zone
							  ,vl_sens_zone
							  ,vl_debut_zone
							  ,vl_fin_zone
							  ,vl_type_zone
							  ,vl_groupe_tunnel
							  ,vl_PRPSTT
							  ,vl_inhibition_zone
							  , vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao352 : appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao352: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier pa_Fd 
       ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
       */
       
       {  
          *pa_Resultat = 0;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd 
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = 0;

return XDC_OK;

}   

/* Fin de la fonction  */


/*E*/
/* Fichier : $Id: xzao113.c,v 1.4 2018/10/26 13:13:02 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/10/26 13:13:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao113.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	18 Oct 1994	: Creation
* volcic	28 Oct 1994	: Modification de recuperation des parametres de la structure (v1.2)
* JPL	13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.3
* JPL	26/10/18 : Lecture de l'indicateur 'Supprime' en configuration (DEM 1305)  1.4
* GGY	25/09/23 : Ajout @ip et port esclave et maitre (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao113.c,v 1.4 2018/10/26 13:13:02 devgfi Exp $ : xzao113" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des PIRAU et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao113_Lire_Config_PIRAU ( 	FILE  		*pa_Fd,
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
*       XZAOT_ConfPIRAU 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao113.c,v 1.4 2018/10/26 13:13:02 devgfi Exp $ : xzao113_Lire_Config_PIRAU" ;
   
   XZAOT_ConfPIRAU	vl_PIRAU;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao113: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant � pa_Fd
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
             ** Extraction des differents champs de vl_District
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PIRAU.Nom, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PIRAU.NomServeur, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PIRAU.LiaisonMaitre = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PIRAU.LiaisonEsclave = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Rgs, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PIRAU.RGS, vl_Texte_out );



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao113 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_PIRAU.Supprime = atoi ( vl_Texte_out );
              
            vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
            if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_AdresseIP, vl_Texte_out ) ) != XDC_OK )
            { 
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie AdresseIP  non valide");
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
               return ( XDC_NOK );
            }
            strcpy ( vl_PIRAU.AdresseIpMaitre, vl_Texte_out );

            vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
            if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
            { 
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie Port IP Maitre non valide");
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
               return ( XDC_NOK );
            }
            vl_PIRAU.PortIpMaitre = atol ( vl_Texte_out );

			             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
            if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_AdresseIP, vl_Texte_out ) ) != XDC_OK )
            { 
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie AdresseIP  non valide");
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
               return ( XDC_NOK );
            }
            strcpy ( vl_PIRAU.AdresseIpEsclave, vl_Texte_out );

            vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
            if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
            { 
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie Port IP Maitre non valide");
               XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
               return ( XDC_NOK );
            }
            vl_PIRAU.PortIpEsclave = atol ( vl_Texte_out );

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_PIRAU, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao113: echec de l'appel de la fonction utilisateur");
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
          *pa_Resultat = NULL;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd 
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = NULL;

return XDC_OK;

}   

/* Fin de la fonction xzao113_Lire_Config_PIRAU */


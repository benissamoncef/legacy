/*E*/
/*Fichier : $Id: xzao506.c,v 1.16 2016/07/13 12:43:14 devgfi Exp $      Release : $Revision: 1.16 $        Date : $Date: 2016/07/13 12:43:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao506.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
*
* HISTORIQUE :
*
* ABE 02/08/2022 : Creation NIC et IS localisation tunnel DEM-SAE403
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao000.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao506.c,v 1.16 2016/07/13 12:43:14 devgfi Exp $ : xzao506.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des autoroutes et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int xzao506_Lire_Config_NIC_IS(FILE     	*pa_Fd,
				        XDY_FonctionInt pa_FonctionUtil,
				        XDY_Entier      *va_Resultat) 


/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fichier		pa_Fd_In
* XDY_Fonction		pa_FonctionUtilisateur
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*va_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*       XDY_Nom    contenu de ligne
*       XDY_Entier NumLigne
*    La fonction utilisateur vl_retcodeourne XDC_OK ou XDC_NOK -> arvl_retcode de la lecture
*    et code de vl_retcodeour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   char			vl_Lig[2506];
   char			*vl_Texte;
   char			vl_Texte_out[2506];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   int			vl_NbCarLu = 0;
   XDY_Entier		vl_Resultat_out = 0;
   XZAOT_ConfNIC_IS vl_NIC_IS;
   
   static char *version = "$Id: xzao506.c,v 1.16 2016/07/13 12:43:14 devgfi Exp $ xzao506_Lire_Config_NIC_IS" ;
				        

 
   /* Test sur le file descriptor passe en parametre d'entree */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao506: fichier non existant");
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
          
          *va_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presenece du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != 0) 

          {
             
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506_Lire_Config_NIC_IS lecture de Numero non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }  
	          vl_NIC_IS.Numero = atoi(vl_Texte_out);
               
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506_Lire_Config_NIC_IS lecture de Nom non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_NIC_IS.Nom, vl_Texte_out );
                  
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506_Lire_Config_NIC_IS lecture de type non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }  
	          vl_NIC_IS.Type = atoi(vl_Texte_out);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506_Lire_Config_NIC_IS lecture de autoroute non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }  
             strcpy ( vl_NIC_IS.Autoroute, vl_Texte_out );
                   
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506_Lire_Config_NIC_IS lecture de Pr non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }  
	          vl_NIC_IS.PR = atoi(vl_Texte_out);
            
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

            if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506_Lire_Config_NIC_IS lecture de sens non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao506 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }  
	          vl_NIC_IS.Sens = atoi(vl_Texte_out);

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_NIC_IS, vl_num_ligne ) ) != XDC_OK )
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

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao506: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier pa_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
          *va_Resultat = NULL;
          return ( XDC_OK );
       }   
 
   }
 
   /* Fin de la boucle de lecture du fichier pa_Fd */


   /* Retour du status XDC_OK et du parametre *va_Resultat = NULL */
   
*va_Resultat = NULL;

return XDC_OK;

}     

/* Fin de la fonction xzao506_Lire_Config_NIC_IS */



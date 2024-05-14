/*E*/
/* Fichier : $Id: xzao507sp.prc,v 1.8 2021/10/07 17:45:31 pc2dpdy Exp $        $Revision: 1.8 $        $Date: 2021/10/07 17:45:31 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao507sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Insert IMU en base EQT_GEN
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 02/08/2022 : Creation NIC et IS DEM-SAE403
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao507sp.h" */

/* static char *version = "@(#)xzao507sp.prc	1.1 1.1 07/20/28 : xzao507" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X
------------------------------------------------------
* SERVICE RENDU : 
* Ajout d'une NIC ou IS dans la table RES_NIC_IS
*
* SEQUENCE D'APPEL :
* SP	XZAO507_Ajouter_NIC_IS
*
* Arguments en entree
* XDY_Basedd	 va_Basedd_in
* XDY_Entier	 va_Numero_in
* XDY_NomEqt	 va_Nom_in
* XDY_Mot   	 va_Type_in
* XDY_NomAuto	 va_Autoroute_in
* XDY_PR	       va_PR_in
* XDY_Sens	    va_Sens_in
*
* Arguments en sortie
* XDC_Entier 	va_Resultat_out    
*
------------------------------------------------------
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO507' and type = 'P')
	drop procedure XZAO507
go

create procedure XZAO507
   @va_Basedd_in        char(3),
   @va_Numero_in        int,
   @va_Nom_in		    char(10),
   @va_Type_in          smallint,
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_Resultat_out	int = null output
   as 

------------------------------------------------------*/
   /*A
   ** Definition des variables globales
   */
declare
   @vl_Basedd     char(3),
   @vl_Numero     int,
   @vl_Autoroute 	tinyint

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_Numero_in = null or
      @va_Nom_in = null or @va_Type_in = null or
      @va_Autoroute_in = null or @va_PR_in = null or
      @va_Sens_in = null
      return XDC_ARG_INV
   else   
   begin
      
      /*A
      **  Insertion du nom de l'objet argument dans la table RES_NIC_IS de la base
      **  passee en argument, si celui ci n'existe pas
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
            select   @vl_Numero = numero
            from CFG..RES_NIC_IS where numero = @va_Numero_in
#ifdef CI

      else
         select   @vl_Numero = numero
         from CFT..RES_NIC_IS where numero = @va_Numero_in
      
#endif


      /* Recupère le numero d'autoroute */
      select @vl_Autoroute = numero from CFG..RES_AUT where nom = @va_Autoroute_in

      /* si le nom d'autoroute n'éxiste pas */
      if @vl_Autoroute = null
         return XDC_ARG_INV

      /*A
      ** L'entrée n'existe pas dans la base
      */
      
      if @vl_Numero = null
      begin
         if @va_Basedd_in = XDC_BASE_CFG
         begin   
            select @vl_Numero = max(numero) from CFG..RES_NIC_IS
            
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFG..RES_NIC_IS
            values ( @vl_Numero + 1,@va_Nom_in,@va_Type_in,@vl_Autoroute,@va_PR_in,@va_Sens_in)
         end   

#ifdef CI

         else
         begin   
            select @vl_Numero = max(numero) from CFT..RES_NIC_IS
            
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFT..RES_NIC_IS
            values ( @vl_Numero + 1,@va_Nom_in,@va_Type_in,@vl_Autoroute,@va_PR_in,@va_Sens_in)
         end   

#endif
         
         /*A
         ** Erreur d'insertion
         */
                     
         if @@rowcount <> 1
         begin
                     
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_AJOUT
            */
         
            select @va_Resultat_out = XDC_ECHEC
                        
            return XDC_NOK
         end
         
         /*A
         ** Affectation du parametre de sortie Resultat a XDC_AJOUT
         */
         
         select @va_Resultat_out = XDC_AJOUT
         
      end

      /*A
      ** L'entrée éxiste pas dans la base
      */

         if @va_Basedd_in = XDC_BASE_CFG
         begin
              								          								
            update CFG..RES_NIC_IS set nom=@va_Nom_in,type=@va_Type_in,autoroute=@vl_Autoroute,PR=@va_PR_in,sens=@va_Sens_in where numero = @va_Numero_in
         end   

#ifdef CI

         else
         begin
                        
            update CFT..RES_NIC_IS set nom=@va_Nom_in,type=@va_Type_in,autoroute=@vl_Autoroute,PR=@va_PR_in,sens=@va_Sens_in where numero = @va_Numero_in
         end   

#endif
         /*A
         ** Erreur de modification
         */
                     
         if @@rowcount <> 1
         begin
                     
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_AJOUT
            */
         
            select @va_Resultat_out = XDC_ECHEC
                        
            return XDC_NOK
         end
         
         /*A
         ** Affectation du parametre de sortie Resultat a XDC_AJOUT
         */
         
         select @va_Resultat_out = XDC_MODIF

   end
      
   return XDC_OK
         
go  

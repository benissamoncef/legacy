/*E*/
/*Fichier : $Id: xzaa31sp.prc,v 1.1 2019/11/18 17:37:26 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/11/18 17:37:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	16/09/19	Creation COCKPIT LOT27 DEM1351 V1.0	
------------------------------------------------------*/

*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une alerte a la liste des alertes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAA31' and type = 'P')
	drop procedure XZAA31
go


create procedure XZAA31
   @va_Horodate_in	datetime = null,
   @va_TypeEqt_in	tinyint = null,
   @va_NumEqt_in	smallint = null,
   @va_Type_in		tinyint = null,
   @va_Texte_in		char(100),
   @va_Autoroute_in     char(10),
   @va_PR_deb_in        int,
   @va_PR_fin_in        int,
   @va_Sens_in          tinyint

   
   as 

/* 
*
* SP	XZAA01SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_NumEqt_in
* XDY_TypeAlerte	va_Type_in
* XDY_TexteAlerte	va_Texte_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Ajoute une alerte a la liste des alertes
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
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_Eqt 		smallint,
   @vl_Type		tinyint,
   @vl_NomType		char(25),
   @vl_Numero		int,
   @vl_Serveur		tinyint,
   @vl_Cle		tinyint,
   @vl_Evt		int,
   @vl_Autoroute	tinyint,
   @vl_PR		int,
   @vl_Sens		tinyint,
   @vl_Creation		tinyint,
   @vl_NomEqt		char(10),
   @vl_Portion             smallint,
   @vl_Translation         int,
   @vl_texte		char(100),
   @vl_index		tinyint,
   @vl_nb		int

       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = null
   or @va_TypeEqt_in = null
   or @va_NumEqt_in = null
   or @va_Type_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Si le texte est null => mis a ""
      */
      
      if @va_Texte_in = null
         select @va_Texte_in = ' '
      
      select @vl_Eqt = numero from CFG..EQT_GEN
        where type   = @va_TypeEqt_in
          and numero = @va_NumEqt_in
          
      select @vl_Type = numero, @vl_NomType = nom from CFG..TYP_ALT
        where numero = @va_Type_in
          
      /*A
      ** L'equipement et le type d'alerte existent 
      */
      
      if @vl_Eqt != null and @vl_Type != null
      begin
         
        /* recherche le site local */
        select @vl_Serveur = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Serveur is null
                return XDC_NOK
         
         
         
        /*recup numero autoroute*/
        select @vl_Autoroute=numero from CFG..RES_AUT
        where nom = @va_Autoroute_in

        if @@rowcount=0
        begin
                return XDC_ARG_INV
        end

	if @va_Type_in=XZAAC_ALERTE_COMMUNAUTAIRE
	begin
		select @vl_index=patindex("%#%",@va_Texte_in)
		select @vl_texte = substring(@va_Texte_in,1,@vl_index-1) + '#%'
		select @vl_nb = count(*) from EXP..ALT_EXP
		where type = @va_Type_in
		and equipement = @va_NumEqt_in
		and qualification = 0
		and type_equipement = @va_TypeEqt_in
		and texte like @vl_texte

		if @vl_nb > 0
		begin
			return XDC_OK
		end
	end


         begin tran
         select @vl_Numero = max ( numero ) from EXP..ALT_EXP where sit=@vl_Serveur
         
         if @vl_Numero = null
            select @vl_Numero = 0
         
         select @vl_Numero = @vl_Numero + 1

         insert into EXP..ALT_EXP
         values ( @vl_Numero, @va_TypeEqt_in, @va_NumEqt_in, @va_Type_in, @va_Horodate_in, @va_Texte_in, XDC_FAUX, null, @vl_Serveur, null )
         commit tran

         insert into EXP..ALT_LOC (numero, sit, autoroute, PR, sens)
         values (@vl_Numero,  @vl_Serveur, @vl_Autoroute, @va_PR_in, @va_Sens_in)


         select @vl_PR          = @va_PR_in,
                @vl_Sens        = @va_Sens_in
         
         select @vl_NomEqt	= nom
         from CFG..EQT_GEN
         where type   = @va_TypeEqt_in
           and numero = @va_NumEqt_in
         
         select  @vl_Portion=numero,
            @vl_Translation=10000*(@vl_PR-PR_debut)/(PR_fin-PR_debut)
                        from CFG..RES_POR
                        where   autoroute=@vl_Autoroute and
                                PR_debut<@va_PR_in and
                                PR_fin>=@va_PR_in

         select @vl_Creation = XDC_VRAI
         
            /* PRA
	    if @@servername = XDC_SQLVC
            */
	    if @vl_Serveur = XDC_VC
	    
               exec TASRV_VC...AARP11 	@vl_Numero, 
            				@va_Horodate_in, 
            				@va_Type_in, 
            				@vl_NomType, 
            				@va_TypeEqt_in, 
            				@va_NumEqt_in, 
            				@vl_NomEqt, 
            				@va_Texte_in,
            				@vl_Autoroute, 
            				@vl_PR, 
            				@vl_Sens, 
            				@vl_Creation,
            				@vl_Serveur,
                                       @vl_Portion,
                                        @vl_Translation

            else
               /* PRA
	       if @@servername = XDC_SQLDP
               */
               if @vl_Serveur = XDC_DP
		begin
		select " TASRV_DP...AARP11"
                  exec TASRV_DP...AARP11 	@vl_Numero, 
            					@va_Horodate_in, 
            					@va_Type_in, 
            					@vl_NomType, 
            					@va_TypeEqt_in, 
            					@va_NumEqt_in, 
            					@vl_NomEqt, 
            					@va_Texte_in,
            					@vl_Autoroute, 
            					@vl_PR, 
            					@vl_Sens, 
            					@vl_Creation,
						@vl_Serveur,
                                        @vl_Portion,
                                        @vl_Translation
		end
                  
            else
                  return ( XDC_NOK )   
      end
       
   end
     
   return ( XDC_OK )
         
go  

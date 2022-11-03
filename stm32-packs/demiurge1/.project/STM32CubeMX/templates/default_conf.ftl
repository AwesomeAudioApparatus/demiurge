[#ftl]
/**
  ******************************************************************************
  * File Name          : ${name}
  * Description        : This file provides code for the configuration
  *                      of the ${name} instances.
  ******************************************************************************
[@common.optinclude name="license.txt"/][#--include License text --]
  ******************************************************************************
  */
[#assign s = name]
[#assign toto = s?replace(".","__")]
[#assign dashReplace = toto?replace("-","_")]
[#assign inclusion_protection = dashReplace?upper_case]
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _${inclusion_protection}_
#define _${inclusion_protection}_

#ifdef __cplusplus
 extern "C" {
#endif

[#if includes??]
[#list includes as include]
#include "${include}"
[/#list]
[/#if]

[#-- SWIPdatas is a list of SWIPconfigModel --]  
[#list SWIPdatas as SWIP]  
[#-- Global variables --]
[#if SWIP.variables??]
	[#list SWIP.variables as variable]
extern ${variable.value} ${variable.name};
	[/#list]
[/#if]

[#-- Global variables --]

[#assign instName = SWIP.ipName]   
[#assign fileName = SWIP.fileName]   
[#assign version = SWIP.version]   

/**
	MiddleWare name : ${instName}
	MiddleWare fileName : ${fileName}
	MiddleWare version : ${version}
*/
[#if SWIP.defines??]
	[#list SWIP.defines as definition]	
/*---------- [#if definition.comments??]${definition.comments} [/#if] -----------*/
#define ${definition.name} #t#t ${definition.value} 
[#if definition.description??]${definition.description} [/#if]
	[/#list]
[/#if]



[/#list]


#ifdef __cplusplus
}
#endif
#endif /*_${inclusion_protection}_ */

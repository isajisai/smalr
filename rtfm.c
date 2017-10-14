//*******************************************************************//
//	TITLE: WHY SILAS IS WRONG ABOUT CONVENTIONS
//	
//	DESCRIPTION: ALL CODE FOLLOWS THESE RULES:
//	
//	(RULES WITH EXCEPTIONS ARE DEMARCATED WITH A *)
//							~~~~~~~~~~~~
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// $$$$$$$$$$$$$$$$ 	1.) R . T . F . M .	   $$$$$$$$$$$$$$$$$$$$$$
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//							~~~~~~~~~~~~
//		 2) CODE MUST NOT BE CAMEL CASE
//		 3) CODE MUST USE 4-SPACE TABS
//		*4) ALL LOOPS AND FUNCTIONS MUST PUT BRACES ON SEPARATE LINES
//		
//	EX: >>> static void print_add(int a, int b) 
//		>>> {
//		>>>		printf("%i + %i = %i", a, b, a + b);
//		>>>	}
//
//	EX: >>> for(;;)
//		>>> {
//		>>> 	...
//		>>>		while(...)
//		>>>		{
//		>>>			if(...)
//		>>>			{
//		>>>				...
//		>>>			}
//		>>>			...
//		>>>		}
//		>>>		...
//		>>>	}
//		
//		EXCEPTIONS: SINGLE-LINE OR PROCEDURAL CONDITIONALS OR LOOPS
//		MAY HAVE CLAUSES SEPARATED BY A NEW LINE AND A TAB
//
//  EX: >>> switch(...)
//      >>> {
//      >>> 	case ...:
//      >>>     {
//      >>>     	int i;
//      >>>         break;
//      >>>     }
//      >>>     
//      >>>     case ...:
//      >>>     	break;
//      >>>         
//      >>>     case ...:
//      >>>     	break;
//      >>> }
//
//	EX:	>>> if(...)
//		>>> 	break;
//
// 		 5) ALL STRUCTS MUST BE STRUCTURE-PACKED:
//			i) INT's AND POINTERS MUST BE GROUPED TOGETHER 
//			ii) 
//	
//  NAMING CONVENTIONS:
//    1. MACROS HAVE ALL CAPITAL LETTERS
//    2. TYPEDEFS USE CAMEL CASE, AND DO NOT HAVE "_t" AT THE END
//    3. FUNCTIONS USE UNDERSCORE NAMING
//    4. LOCAL VARIABLES USE UNDERSCORE NAMING
//    5. GLOBAL USE UNDERSCORE NAMING
//
//  ORDER OF THINGS:
//    1. FILE COMMENT BLOCK
//    2. STANDARD LIBRARY HEADER INCLUDES
//    3. OTHER HEADER INCLUDES
//    4. MACRO DEFINITIONS
//    5. TYPEDEFS AND ENUMS
//    6. GLOBAL VARIABLES
//    7. FUNCTION DECLARATIONS
//    8. PUBLIC FUNCTIONS
//    9. LOCAL (STATIC) FUNCTIONS
//
//
//***************************************************************//

#define kek 420691337

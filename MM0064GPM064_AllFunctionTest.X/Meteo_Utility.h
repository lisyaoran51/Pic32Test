/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef METEO_UTILITY_H    /* Guard against multiple inclusion */
#define METEO_UTILITY_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdbool.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
#define EXAMPLE_CONSTANT 0


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    typedef struct _example_struct_t {
        /* Describe structure member. */
        int some_number;

        /* Describe structure member. */
        bool some_flag;

    } example_struct_t;


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

      @Summary
        Brief one-line description of the function.

      @Description
        Full description, explaining the purpose and usage of the function.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Precondition
        List and describe any required preconditions. If there are no preconditions,
        enter "None."

      @Parameters
        @param param1 Describe the first parameter to the function.
    
        @param param2 Describe the second parameter to the function.

      @Returns
        List (if feasible) and describe the return values of the function.
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Describe any special behavior not described above.
        <p>
        Any additional remarks.

      @Example
        @code
        if(ExampleFunctionName(1, 2) == 0)
        {
            return 3;
        }
     */
    void Meteo_Memcpy(void *dest, void *src, size_t n)
    {
       // Typecast src and dest addresses to (char *)
       char *csrc = (char *)src;
       char *cdest = (char *)dest;

       // Copy contents of src[] to dest[]
       for (int i=0; i<n; i++)
           cdest[i] = csrc[i];
    }
    
    // https://aticleworld.com/memset-in-c/
    void Meteo_Memset(void *s, int c, size_t n)
    {
        unsigned char* p=s;
        while(n--)
        {
            *p++ = (unsigned char)c;
        }
    }
    
    // https://aticleworld.com/how-to-use-strncpy-and-how-to-write-your-own-strncpy/
    char* Meteo_Strncpy(char* dest, const char* src, unsigned int n)
    {
        // return if dest and src is NULL
        if ((dest == NULL) || (src == NULL))
            return NULL;
        // take a pointer pointing to the beginning of dest string
        char* start = dest;
        // copy first n characters of C-string pointed by src
        // into the array pointed by dest
        while (*src && n--)
        {
            *dest++ = *src++;
            //dest++;
            //src++;
        }
        // null terminate dest string
        //*dest = '\0';
        return start;
    }
    #include <string.h>
    // Function to implement `strncat()` function in C
    char* Meteo_Strncat(char* destination, const char* source, size_t num)
    {
        // make `ptr` point to the end of the destination string
        char* ptr = destination + strlen(destination);

        // Appends characters of the source to the destination string
        while (*source != '\0' && num--) {
            *ptr++ = *source++;
        }

        // null terminate destination string
        *ptr = '\0';

        // destination string is returned by standard `strncat()`
        return destination;
    }

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */

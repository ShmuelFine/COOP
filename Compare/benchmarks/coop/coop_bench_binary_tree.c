 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 #include "COOP.h"
 #include "BinaryTree.h"


 FUN_IMPL(main, int argc, char** argv)
 {
     FUN(init_global_memory) 0, HEAP_BASED_MEMORY CALL;

     const char* phase = (argc > 1) ? argv[1] : "all";
     MEM_SIZE_T  N = (argc > 2) ? (MEM_SIZE_T)strtoull(argv[2], NULL, 10) : (MEM_SIZE_T)100000;

     CREATE(BTree_int, bt) CALL;

     // ---- insert ----
     if (strcmp(phase, "insert") == 0)
     {
         FOR(MEM_SIZE_T i = 0; i < N; ++i)
         {
             MFUN(&bt, insert), (int)i CALL;
         }
         END_LOOP;
     }
     // ---- remove ----
     else if (strcmp(phase, "remove") == 0)
     {
         FOR(MEM_SIZE_T i = 0; i < N; ++i)
         {
             MFUN(&bt, insert), (int)i CALL;
         }
         END_LOOP;

         FOR(MEM_SIZE_T i = 0; i < N; ++i)
         {
             bool removed = false;
             MFUN(&bt, remove), (int)i, & removed CALL;
         }
         END_LOOP;
     }
     // ---- size ----
     else if (strcmp(phase, "size") == 0)
     {
         MEM_SIZE_T s = 0;
         FOR(int i = 0; i < N; i++) {
             MFUN(&bt, get_size), & s CALL;
         }END_LOOP;
     }
     // ---- empty (API is_empty) ----
     else if (strcmp(phase, "empty") == 0)
     {
         bool is_empty = false;
         FOR(int i = 0; i < N; i++) {
             MFUN(&bt, is_empty), & is_empty CALL;
         }END_LOOP;
     }
     // ---- all ----
     else
     {
         MEM_SIZE_T s = 0;
         bool is_empty = false;

         FOR(MEM_SIZE_T i = 0; i < N; ++i)
         {
             MFUN(&bt, insert), (int)i CALL;
             MFUN(&bt, get_size), & s CALL;
             MFUN(&bt, is_empty), & is_empty CALL;
         }
         END_LOOP;

         FOR(MEM_SIZE_T i = 0; i < N; ++i)
         {
             bool removed = false;
             MFUN(&bt, remove), (int)i, & removed CALL;
         }
         END_LOOP;
     }

     DESTROY(&bt);
     RETURN(0);
 }
 END_FUN


/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/

/************************************************************************
 *
 *	Task management.
 *
 *       This module has the following functions.
 *       (1) Allocate/free a task object.
 *       (2) Enqueue/decuque a task object.
 *
 *************************************************************************/

#include <stdio.h>


#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
extern pthread_t PThreadTable[];
;


/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/


/*  This file contains many constant definitions that control the execution 
of the program, as well as lobal data structure declarations */

#ifndef _RADIOSITY_H
#define _RADIOSITY_H

#include <math.h>

/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/

/**************************************************************
*
*       Definitions relevant to parallel processing
*
***************************************************************/

#ifndef _PARALLEL_H
#define _PARALLEL_H



/***************************************************************************
*
*    Shared lock variable
*
*    Some machines provide only a limited number of lock variables. This
*    data structure allows sharing of these lock variables.
*    The shared locks are divided into 2 segments so that different types of
*    objects are given different locks. 
*
****************************************************************************/

typedef struct
{
    pthread_mutex_t (lock);
} Shared_Lock ;

#define SHARED_LOCK_SEG_SIZE (MAX_SHARED_LOCK / 2)

#define SHARED_LOCK_SEG0 (0)
#define SHARED_LOCK_SEG1 (1)
#define SHARED_LOCK_SEGANY (2)

extern void init_sharedlock() ;
extern Shared_Lock *get_sharedlock() ;

/****************************************************************************
*
*    Memory Consistency Model of the machine
*
*    Some macro changes its behavior based on the memory consistency model
*
*
*****************************************************************************/

/* Set one(1) to the model used in the machine.  Set only one of these
at a time */

#define MEM_CONSISTENCY_RELEASE    (0)
#define MEM_CONSISTENCY_WEAK       (0)
#define MEM_CONSISTENCY_PROCESSOR  (1)

#endif



/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/


#ifndef _PATCH_H
#define _PATCH_H


/************************************************************************
*
*     Constants
*
*************************************************************************/

#define F_COPLANAR  (5.0e-2)     /* H(P) < F_COPLANAR then P is on the plane */
#define N_VISIBILITY_TEST_RAYS  (10)	/* number of "random", "magic" rays fired 
between patches to test visibility */

#define FF_GEOMETRY_ERROR (1.0)		/* FF relative error due to Fdf approx
and cosine approx of angle */
#define FF_GEOMETRY_VARIANCE (1.0)	/* FF relative varance with in elem */
#define FF_VISIBILITY_ERROR (1.0 / N_VISIBILITY_TEST_RAYS)



/************************************************************************
*
*     Intersection code
*
*************************************************************************/

#define POINT_POSITIVE_SIDE   (1)
#define POINT_NEGATIVE_SIDE   (2)
#define POINT_ON_PLANE        (0)

#define P1_POSITIVE    (1)
#define P1_NEGATIVE    (2)
#define P2_POSITIVE    (4)
#define P2_NEGATIVE    (8)
#define P3_POSITIVE    (16)
#define P3_NEGATIVE    (32)
#define ANY_POSITIVE   (P1_POSITIVE | P2_POSITIVE | P3_POSITIVE)
#define ANY_NEGATIVE   (P1_NEGATIVE | P2_NEGATIVE | P3_NEGATIVE)
#define POSITIVE_SIDE(code) (((code) & ANY_NEGATIVE) == 0)
#define NEGATIVE_SIDE(code) (((code) & ANY_POSITIVE) == 0)
#define INTERSECTING(code)  (   ((code) & ANY_NEGATIVE) \
&& ((code) & ANY_POSITIVE) )
#define P1_CODE(code)  (code & 3)
#define P2_CODE(code)  ((code >> 2) & 3)
#define P3_CODE(code)  ((code >> 4) & 3)

/************************************************************************
*
*     Visibility Testing
*
*************************************************************************/

#define      VISIBILITY_UNDEF      ((float)-1.0)
#define      PATCH_CACHE_SIZE      (2)        /* The first two cache entries
covers about 95% of the total cache hits, so using
more doesn't help too much. */

extern float visibility() ;
extern void  compute_visibility_values() ;
extern void  visibility_task() ;

/************************************************************************
*
*     Refinement Advice
*
*************************************************************************/

#define _NO_INTERACTION          (1)
#define _NO_REFINEMENT_NECESSARY (2)
#define _REFINE_PATCH_1          (4)
#define _REFINE_PATCH_2          (8)
#define _NO_VISIBILITY_NECESSARY (16)

#define NO_INTERACTION(c)          ((c) & _NO_INTERACTION)
#define NO_REFINEMENT_NECESSARY(c) ((c) & _NO_REFINEMENT_NECESSARY)
#define REFINE_PATCH_1(c)          ((c) & _REFINE_PATCH_1)
#define REFINE_PATCH_2(c)          ((c) & _REFINE_PATCH_2)
#define NO_VISIBILITY_NECESSARY(c) ((c) & _NO_VISIBILITY_NECESSARY)


/************************************************************************
*
*     Vertex    -  3D coordinate
*
*************************************************************************/

typedef struct {
    float x, y, z ;
} Vertex ;


extern float vector_length() ;
extern float distance() ;
extern float normalize_vector() ;
extern float inner_product() ;
extern void cross_product() ;
extern float plane_normal() ;
extern void center_point(), four_center_points() ;
extern void print_point() ;


/************************************************************************
*
*     Color (R,G,B)
*
*************************************************************************/

typedef struct {
    float r, g, b ;
} Rgb ;



extern void print_rgb() ;



/************************************************************************
*
*     Element Vertex
*
*     ElementVertex represents a vertex of an element. A vertex structure
*     is shared by those elements which contain the vertex as part of their
*     vertex list.
*
*************************************************************************/

typedef struct _elemvertex {
    Vertex p ;			  /* Coordinate of the vertex */
    Rgb    col ;			  /* Color of the vertex */
    float  weight ;			  /* weight */
    Shared_Lock *ev_lock ;
} ElemVertex ;


#define N_ELEMVERTEX_ALLOCATE (16)
extern ElemVertex *get_elemvertex() ;
extern ElemVertex *create_elemvertex() ; /* Constructor */
extern void         init_elemvertex() ;	  /* Initialize free buffer */


/************************************************************************
*
*     Edge
*
*     Edge represents each edge of the element. Two adjacent elements
*     share the same edge. As an element is subdivided, the edge is also
*     subdivided. The edges form a binary tree, which can be viewed as a
*     projection of the element subdivision along an edge of the element.
*     In other words, the edge structure binds elements at the same height.
*     Note that the vertices may appear in reverse order in the edge structure
*     with respect to the order in the patch/element definition.
*
*************************************************************************/

typedef struct _edge {
    ElemVertex   *pa, *pb ;
    struct _edge *ea, *eb ;		  /* Edge (A-center) and (center-B) */
    Shared_Lock  *edge_lock ;	          /* Use segment0 */
} Edge ;


#define N_EDGE_ALLOCATE (16)
extern Edge *get_edge() ;
extern Edge *create_edge() ;		  /* Constructor */
extern void subdivide_edge() ;
extern void init_edge() ;		  /* Initialize free buffer */
extern void foreach_leaf_edge() ;

#define _LEAF_EDGE(e) ((e)->ea == 0)
#define EDGE_REVERSE(e,a,b) ((e)->pa == (b))


/************************************************************************
*
*     Planar equation
*
*     Plane equation (in implicit form) of the triangle patch.
*     A point P on the plane satisfies
*         (N.P) + C = 0
*     where N is the normal vector of the patch, C is a constant which
*     is the distance of the plane from the origin scaled by -|N|.
*
*************************************************************************/

typedef struct {
    Vertex  n ;		          /* Normal vector (normalized) */
    float  c ;			  /* Constant */
    /* Nx*x + Ny*y + Nz*z + C = 0 */
} PlaneEqu ;


extern float plane_equ() ;
extern float comp_plane_equ() ;
extern int point_intersection() ;
extern int patch_intersection() ;
extern void print_plane_equ() ;


/************************************************************************
*
*     Patch (also a node of the BSP tree)
*
*     The Patch represents a triangular patch (input polygon) of the given 
*     geometric model (i.e., room scene). The Patch contains 'per-patch' 
*     information such as the plane equation, area, and color. The Patch also 
*     serves as a node of the BSP tree which is used to test patch-patch 
*     visibility. The Patch points to the root level of the element quad-tree.
*     Geometrically speaking, the Patch and the root represent the same
*     triangle.
*     Although coordinates of the vertices are given by the Edge structure,
*     copies are stored in the Patch to allow fast access to the coordinates
*     during the visibility test.
*     For cost based task distribution another structure, Patch_Cost, is
*     also used. This structure is made separate from the Patch structure
*     since gathering cost statistics is a frequently read/write operation.
*     If it were in the Patch structure, updating a cost would result in
*     invalidation of the Patch structure and cause cache misses during
*     BSP traversal.
*
*************************************************************************/

struct _element ;

typedef struct _patch {
    ElemVertex *ev1, *ev2, *ev3 ;	  /* ElemVertecies of the patch */
    Edge    *e12, *e23, *e31 ;          /* Edges of the patch */
    Vertex   p1, p2, p3 ;		  /* Vertices of the patch */
    PlaneEqu plane_equ ;		  /* Plane equation H(x,y,z) */
    float    area ;			  /* Area of the patch */
    Rgb      color ;			  /* Diffuse color of the patch */
    /*       (reflectance) */
    Rgb      emittance ;	          /* Radiant emmitence */
    
    struct _patch  *bsp_positive ;	  /* BSP tree H(x,y,z) >= 0 */
    struct _patch  *bsp_negative ;	  /*          H(x,y,z) <  0 */
    struct _patch  *bsp_parent ;        /* BSP backpointer to the parent*/
    
    struct _element *el_root ;	  /* Root of the element tree */
    int      seq_no ;		          /* Patch sequence number */
} Patch ;

extern void foreach_patch_in_bsp(), foreach_depth_sorted_patch() ;
extern void define_patch() ;
extern void refine_newpatch() ;
extern Patch *get_patch() ;
extern void init_patchlist() ;
extern void print_patch() ;
extern void print_bsp_tree() ;


typedef struct {
    Patch    *patch ;
    Shared_Lock *cost_lock ;		  /* Cost variable lock */
    int      n_bsp_node ;	          /* Number of BSP nodes visited */
    int      n_total_inter ;	          /* Total number of interactions */
    int      cost_estimate ;            /* Cost estimate */
    int      cost_history[11] ;	  /* Cost history */
} Patch_Cost ;

/* Patch cost:
Visiting a node in BSP tree:  150 cyc (overall)
Gathering ray per interaction: 50 cyc (overall avg) */

#define PATCH_COST(p)          ((p)->n_bsp_node * 3 + (p)->n_total_inter)
#define PATCH_COST_ESTIMATE(p)  ((p)->cost_history[0] \
+ ((p)->cost_history[1] >> 1)\
+ ((p)->cost_history[2] >> 2) )


/************************************************************************
*
*     Element
*
*     The Element represents each node of the quad-tree generated by the 
*     hierarchical subdivision. The Element structure consists of:
*      - pointers to maintain the tree structure
*      - a linear list of interacting elements
*      - radiosity value of the element
*      - pointer to the vertex and edge data structures
*
*     To allow smooth radiosity interpolation across elements, an element
*     shares edges and vertices with adjacent elements.
*
*************************************************************************/

struct _interact ;

typedef struct _element {
    Shared_Lock *elem_lock ;	          /* Element lock variable (seg 1) */
    Patch *patch ;			  /* Original patch of the element */
    
    struct _element *parent ;		  /* Quad tree (parent)          */
    struct _element *center ;		  /*           (center triangle) */
    struct _element *top ;		  /*           (top)             */
    struct _element *left ;		  /*           (left)            */
    struct _element *right ;		  /*           (right)           */
    
    struct _interact *interactions ;	  /* Top of light interaction list */
    int  n_interactions ;		  /* Total # of interactions */
    struct _interact *vis_undef_inter ; /* Top of visibility undef list */
    int  n_vis_undef_inter ;		  /* # of interactions whose visibility
    is not yet calculated */
    Rgb  rad ;			  /* Radiosity of this element
    (new guess of B) */
    Rgb  rad_in ;			  /* Sum of anscestor's radiosity */
    Rgb  rad_subtree ;		  /* Area weighted sum of subtree's
    radiosity (includes this elem) */
    int  join_counter ;		  /* # of unfinished subprocesses */
    
    ElemVertex *ev1, *ev2, *ev3 ;	  /* Vertices of the element */
    Edge       *e12, *e23, *e31 ;	  /* Edges of the element */
    float area ;		          /* Area of the element */
} Element ;


extern void foreach_element_in_patch(), foreach_leaf_element_in_patch() ;
extern void ff_refine_elements() ;
extern void subdivide_element() ;
extern int  element_completely_invisible() ;
extern void process_rays() ;
extern Element *get_element() ;
extern void init_elemlist() ;
extern void print_element() ;

#define _LEAF_ELEMENT(e) ((e)->center == 0)

#if MEM_CONSISTENCY_PROCESSOR
#define LEAF_ELEMENT(e)  _LEAF_ELEMENT((e))
#endif

#if (MEM_CONSISTENCY_RELEASE || MEM_CONSISTENCY_WEAK)
extern int leaf_element() ;
#define LEAF_ELEMENT(e) (leaf_element((e)))
#endif


/************************************************************************
*
*     Interaction
*
*************************************************************************/

typedef struct _interact {
    struct _interact *next ;		  /* Next entry of the list */
    Element *destination ;	          /* Partner of the interaction */
    float   formfactor_out ;		  /* Form factor from this patch  */
    float   formfactor_err ;            /* Error of FF */
    float   area_ratio ;		  /* Area(this) / Area(dest) */
    float   visibility ;		  /* Visibility (0 - 1.0) */
} Interaction ;


extern void foreach_interaction_in_element() ;
extern void compute_interaction(), compute_formfactor() ;
extern void insert_interaction(), delete_interaction() ;
extern void insert_vis_undef_interaction(), delete_vis_undef_interaction() ;
extern void init_interactionlist() ;
extern Interaction *get_interaction() ;
extern void free_interaction() ;
extern void print_interaction() ;

#endif


/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/

/* Header file for model data structures and definitions */

#ifndef _MODEL_H
#define _MODEL_H


/************************************************************************
*
*     Constants
*
*************************************************************************/

#define MODEL_TRIANGLE  (0)
#define MODEL_RECTANGLE (1)
#define MODEL_NULL      (-1)

#define MODEL_TEST_DATA (0)
#define MODEL_ROOM_DATA (1)
#define MODEL_LARGEROOM_DATA (2)


/************************************************************************
*
*     Model descriptor
*
*************************************************************************/

/* General structure of the model descriptor */
typedef struct {
    Rgb   color ;			/* Diffuse color */
    Rgb   emittance ;		        /* Radiant emittance */
    Vertex _dummy[4] ;
} Model ;

/* Triangle */
typedef struct {
    Rgb   color ;			/* Diffuse color */
    Rgb   emittance ;		        /* Radiant emittance */
    Vertex p1, p2, p3 ;
} Model_Triangle ;

typedef Model_Triangle Model_Rectangle ;


typedef struct {
    int type ;
    Model model ;
} ModelDataBase ;



extern int model_selector ;
extern void process_model() ;
extern void init_modeling_tasks() ;

#endif




/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/


#ifndef _TASK_H
#define _TASK_H


/************************************************************************
*
*     Constants
*
*************************************************************************/

#define PAGE_SIZE 4096   /* page size of system, used for padding to 
allow page placement of some logically 
per-process data structures */

/*** Task types ***/
#define TASK_MODELING      (1)
#define TASK_BSP           (2)
#define TASK_FF_REFINEMENT (4)
#define TASK_RAY           (8)
#define TASK_RAD_AVERAGE   (16)
#define TASK_VISIBILITY    (32)


/*** Controling parallelism ***/

#define MAX_TASKGET_RETRY (32)	    /* Max # of retry get_task() can make */
#define N_ALLOCATE_LOCAL_TASK (8)   /* get_task() and free_task() transfer
this # of task objects to/from the
global shared queue at a time */


/************************************************************************
*
*     Task Descriptors
*
*************************************************************************/

/* Decompose modeling object into patches (B-reps) */
typedef struct {
    int   type ;		     /* Object type */
    Model *model ;		     /* Object to be decomposed */
} Modeling_Task ;


/* Insert a new patch to the BSP tree */
typedef struct {
    Patch *patch ;                 /* Patch to be inserted */
    Patch *parent ;		     /* Parent node in the BSP tree */
} BSP_Task ;


/* Refine element interaction based on FF value or BF value */
typedef struct {
    Element *e1, *e2 ;	     /* Interacting elements */
    float   visibility ;           /* Visibility of parent */
    int level ;		     /* Path length from the root element */
} Refinement_Task ;


typedef struct {
    int  ray_type ;
    Element *e ;		     /* The element we are interested in */
} Ray_Task ;


typedef struct {
    Element *e ;		     /* The element we are interested in */
    Interaction *inter ;	     /* Top of interactions */
    int   n_inter ;		     /* Number of interactions */
    void  (*k)() ;		     /* Continuation */
} Visibility_Task ;

/* Radiosity averaging task */

#define RAD_AVERAGING_MODE (0)
#define RAD_NORMALIZING_MODE (1)

typedef struct {
    Element *e ;
    int level ;
    int mode ;
} RadAvg_Task ;



/************************************************************************
*
*     Task Definition
*
*************************************************************************/


typedef struct _task {
    int task_type ;
    struct _task *next ;
    union {
        Modeling_Task   model ;
        BSP_Task        bsp ;
        Refinement_Task ref ;
        Ray_Task        ray ;
        Visibility_Task vis ;
        RadAvg_Task     rad ;
    } task ;
} Task ;


typedef struct {
    char pad1[PAGE_SIZE];	 	/* padding to avoid false-sharing 
    and allow page-placement */	
    pthread_mutex_t (q_lock);
    Task  *top, *tail ;
    int   n_tasks ;
    pthread_mutex_t (f_lock);
    int   n_free ;
    Task  *free ;
    char pad2[PAGE_SIZE];	 	/* padding to avoid false-sharing 
    and allow page-placement */	
} Task_Queue ;


#define TASK_APPEND (0)
#define TASK_INSERT (1)


extern Task *get_task() ;
extern void free_task() ;
extern void enqueue_task() ;
extern Task *dequeue_task(), *dequeue_neighbor_task() ;
#define taskq_length(q)   (q->n_tasks)
#define taskq_top(q)      (q->top)
extern void print_task(), print_taskq() ;
extern int assign_taskq() ;
extern void init_taskq() ;
#define taskq_too_long(q)  ((q)->n_tasks > n_tasks_per_queue)

extern void process_tasks() ;
extern void create_modeling_task(), create_bsp_task() ;
extern void create_ff_refine_task() ;
extern void create_ray_task(), create_radavg_task() ;
extern void create_visibility_tasks() ;
extern void enqueue_ray_task() ;
extern void enqueue_radavg_task() ;
extern int  check_task_counter() ;


#endif


#include "glib.h"
#include "pslib.h"


/****************************************
*
*    Configuration Parameters
*
*****************************************/

/*************************************************************************
*
*    Task scheduling & Load balancing (1)
*       --- Assignment of the patches to the processors
*
*    This macro specifies how patches are assigned to the task queues (ie,
*    processors).
*    - PATCH_ASSIGNMENT_STATIC assigns the same set of patches to the same
*    queue repeatedly over iterations.
*    - PATCH_ASSIGNMENT_COSTBASED assigns patches to queues based on the
*    work associated with those patches in previous iterations, in order
*    to try to balance the initial workload assignment among processors
*    and hence reduce task stealing.
*
**************************************************************************/

#define PATCH_ASSIGNMENT_STATIC    (1)
#define PATCH_ASSIGNMENT_COSTBASED (3)

#if !defined(PATCH_ASSIGNMENT)
#define PATCH_ASSIGNMENT PATCH_ASSIGNMENT_STATIC
#endif


/****************************************
*
*    Constants
*
*****************************************/


#define F_ZERO  (1.0e-6)

#if defined(SIMULATOR)
#define MAX_PROCESSORS (128)	      /* Maximum number of processors
(i.e., processes) created */
#define MAX_TASKQUEUES (128)	      /* Maximum number of task queues */
#define MAX_TASKS    (32768)	      /* # of available task descriptors */
#define MAX_PATCHES  (1024)	      /* # of available patch objects */
#define MAX_ELEMENTS (80000)	      /* # of available element objects */
#define MAX_INTERACTIONS (640000)     /* # of available interaction objs */
#define MAX_ELEMVERTICES  (65536)     /* # of available ElemVertex objs */
#define MAX_EDGES  (65536)            /* # of available Edge objs */
#endif

#if defined(DASH)
#define MAX_PROCESSORS (64)	      /* Maximum number of processors
(i.e., processes) created */
#define MAX_TASKQUEUES (64)	      /* Maximum number of task queues */
#define MAX_TASKS    (32768)	      /* # of available task descriptors */
#define MAX_PATCHES  (1024)	      /* # of available patch objects */
#define MAX_ELEMENTS (80000)	      /* # of available element objects */
#define MAX_INTERACTIONS (640000)     /* # of available interaction objs */
#define MAX_ELEMVERTICES  (65536)     /* # of available ElemVertex objs */
#define MAX_EDGES  (65536)            /* # of available Edge objs */
#endif

#if defined(SGI_GL)
#define MAX_PROCESSORS (8)	      /* Maximum number of processors
(i.e., processes) created */
#define MAX_TASKQUEUES (8)	      /* Maximum number of task queues */
#define MAX_TASKS    (8192)	      /* # of available task descriptors */
#define MAX_PATCHES  (1024)	      /* # of available patch objects */
#define MAX_ELEMENTS (40000)	      /* # of available element objects */
#define MAX_INTERACTIONS (320000)     /* # of available interaction objs */
#define MAX_ELEMVERTICES  (16384)     /* # of available ElemVertex objs */
#define MAX_EDGES  (65536)            /* # of available Edge objs */
#endif

#if defined(SUN4)
#define MAX_PROCESSORS (1)	      /* Maximum number of processors
(i.e., processes) created */
#define MAX_TASKQUEUES (1)	      /* Maximum number of task queues */
#define MAX_TASKS    (1024)	      /* # of available task descriptors */
#define MAX_PATCHES  (1024)	      /* # of available patch objects */
#define MAX_ELEMENTS (20000)	      /* # of available element objects */
#define MAX_INTERACTIONS (160000)     /* # of available interaction objs */
#define MAX_ELEMVERTICES  (16384)     /* # of available ElemVertex objs */
#define MAX_EDGES  (32768)            /* # of available Edge objs */
#endif

#if (!defined(SIMULATOR) && !defined(DASH) && !defined(SGI_GL) && !defined(SUN4))
#define MAX_PROCESSORS (128)	      /* Maximum number of processors
(i.e., processes) created */
#define MAX_TASKQUEUES (128)	      /* Maximum number of task queues */
#define MAX_TASKS    (32768)	      /* # of available task descriptors */
#define MAX_PATCHES  (1024)	      /* # of available patch objects */
#define MAX_ELEMENTS (80000)	      /* # of available element objects */
#define MAX_INTERACTIONS (640000)     /* # of available interaction objs */
#define MAX_ELEMVERTICES  (65536)     /* # of available ElemVertex objs */
#define MAX_EDGES  (65536)            /* # of available Edge objs */
#endif

#define MAX_SHARED_LOCK (3900)	      /* Maximum locks allocated. Objects
share these locks */

#if defined(SGI_GL) || defined(DASH) || defined(SIMULATOR)
#define CLOCK_MAX_VAL (2048*1000000)  /* ANL macro clock max value */
#elif defined(SUN4)
#define CLOCK_MAX_VAL (65536*1000000)  /* ANL macro clock max value */
#else
#define CLOCK_MAX_VAL (2048*1000000)  /* ANL macro clock max value */
#endif



/****************************************
*
*    System defaults
*
*****************************************/

#define DEFAULT_N_PROCESSORS (1)
#define DEFAULT_N_TASKQUEUES (1)
#define DEFAULT_N_TASKS_PER_QUEUE (200)
/* Create new tasks if number of tasks currently
in the queue is less than this number */
#define DEFAULT_N_INTER_PARALLEL_BF_REFINEMENT (5)
/* If the number of interactions is greater than
or equal to this value, BF-refinement is
performed in parallel */
#define DEFAULT_N_VISIBILITY_PER_TASK (4)
/* Number of visibility computations per
visibility task */
#define DEFAULT_AREA_EPSILON (2000.0)
/* If element is smaller than this value,
no further subdivision takes place */
#define DEFAULT_ENERGY_EPSILON (0.005)
/* Terminate radiosity iteration if the
difference of total energy is less than this
value. */
#define DEFAULT_BFEPSILON (0.015)
/* BF refinement threshold level. If the estimated
error of BF (due to FF error and error due to
constant approximation within an element) is
larger than this value, then subdivide */

#define DFLT_VIEW_ROT_X (10.0)
#define DFLT_VIEW_ROT_Y (0.0)
#define DFLT_VIEW_DIST  (8000.0)
#define DFLT_VIEW_ZOOM  (1.0)


/****************************************
*
*    Display mode
*
*****************************************/

#define DISPLAY_FILLED   (0)
#define DISPLAY_SHADED   (1)
#define DISPLAY_EDGEONLY (2)

#define DISPLAY_ALL_INTERACTIONS  (0)
#define DISPLAY_HALF_INTERACTIONS (1)



/****************************************
*
*    Statistical Measure
*
*****************************************/

#define MAX_ITERATION_INFO (16)

struct _element ;

typedef struct
{
    int visibility_comp ;
    int ray_intersect_test ;
    int tasks_from_myq ;
    int tasks_from_otherq ;
    int process_tasks_wait ;
    struct _element *last_pr_task ;
} PerIterationInfo ;


typedef struct
{
    char pad1[PAGE_SIZE];	 	/* padding to avoid false-sharing 
    and allow page-placement */	
    int total_modeling_tasks ;
    int total_def_patch_tasks ;
    int total_ff_ref_tasks ;
    int total_ray_tasks ;
    int total_radavg_tasks ;
    int total_direct_radavg_tasks ;
    int total_interaction_comp ;
    int total_visibility_comp ;
    int partially_visible ;
    int total_ray_intersect_test ;
    int total_patch_cache_check ;
    int total_patch_cache_hit ;
    int patch_cache_hit[PATCH_CACHE_SIZE] ;
    PerIterationInfo per_iteration[ MAX_ITERATION_INFO ] ;
    char pad2[PAGE_SIZE];	 	/* padding to avoid false-sharing 
    and allow page-placement */	
} StatisticalInfo ;

extern void init_stat_info() ;

/****************************************
*
*    Shared data structure definition.
*
*****************************************/

typedef struct
{
    long int rad_start, rad_time, refine_time, wait_time, vertex_time;
} Timing;

typedef struct
{
    
    /* Task queue */
    /* ***** */ int index;
    /* ***** */	pthread_mutex_t (index_lock);
    Task_Queue task_queue[ MAX_TASKQUEUES ] ;
    Task task_buf[ MAX_TASKS ] ;
    
    /* BSP tree root */
    pthread_mutex_t (bsp_tree_lock);
    Patch *bsp_root ;
    
    /* Average radiosity value */
    pthread_mutex_t (avg_radiosity_lock);
    int   converged ;
    Rgb   prev_total_energy ;
    Rgb   total_energy ;
    float total_patch_area ;
    int   iteration_count ;
    
    /* Computation cost estimate */
    pthread_mutex_t (cost_sum_lock);
    int cost_sum ;
    int cost_estimate_sum ;
    Patch_Cost patch_cost[ MAX_PATCHES ] ;
    
    /* Barrier */
    
struct {
	pthread_mutex_t	mutex;
	pthread_cond_t	cv;
	unsigned long	counter;
	unsigned long	cycle;
} (barrier);

    
    /* Private varrier */
    int pbar_count ;
    pthread_mutex_t (pbar_lock);
    
    /* Task initializer counter */
    int task_counter ;
    pthread_mutex_t (task_counter_lock);
    
    /* Resource buffers */
    pthread_mutex_t (free_patch_lock);
    Patch *free_patch ;
    int   n_total_patches ;
    int   n_free_patches ;
    Patch patch_buf[ MAX_PATCHES ] ;
    
    pthread_mutex_t (free_element_lock);
    Element *free_element ;
    int     n_free_elements ;
    Element element_buf[ MAX_ELEMENTS ] ;
    
    pthread_mutex_t (free_interaction_lock);
    Interaction *free_interaction ;
    int         n_free_interactions ;
    Interaction interaction_buf[ MAX_INTERACTIONS ] ;
    
    pthread_mutex_t (free_elemvertex_lock);
    int         free_elemvertex ;
    ElemVertex  elemvertex_buf[ MAX_ELEMVERTICES ] ;
    
    pthread_mutex_t (free_edge_lock);
    int   free_edge ;
    Edge  edge_buf[ MAX_EDGES ] ;
    
    Shared_Lock sh_lock[ MAX_SHARED_LOCK ] ;
    
    StatisticalInfo stat_info[ MAX_PROCESSORS ] ;
    
} Global ;


/****************************************
*
*    Global variables
*
*****************************************/

extern Timing **timing ;
extern Global *global ;
extern int    n_processors ;
extern int    n_taskqueues ;
extern int    n_tasks_per_queue ;

extern int    N_inter_parallel_bf_refine ;
extern int    N_visibility_per_task ;
extern float  Area_epsilon ;
extern float  Energy_epsilon ;
extern float  BFepsilon ;

extern int batch_mode, verbose_mode ;
extern int taskqueue_id[] ; 

extern long time_rad_start, time_rad_end, time_process_start[] ;


/****************************************
*
*    Global function names & types
*
*****************************************/

extern void init_global() ;
extern void init_visibility_module() ;

extern void radiosity_averaging() ;
extern void setup_view() ;
extern void display_scene() ;
extern void display_patch(), display_patches_in_bsp_tree() ;
extern void display_element(), display_elements_in_patch() ;
extern void display_elements_in_bsp_tree() ;
extern void display_interactions_in_element() ;
extern void display_interactions_in_patch() ;
extern void display_interactions_in_bsp_tree() ;

extern void ps_display_scene() ;
extern void ps_display_patch(), ps_display_patches_in_bsp_tree() ;
extern void ps_display_element(), ps_display_elements_in_patch() ;
extern void ps_display_elements_in_bsp_tree() ;
extern void ps_display_interactions_in_element() ;
extern void ps_display_interactions_in_patch() ;
extern void ps_display_interactions_in_bsp_tree() ;

extern void print_statistics() ;
extern void print_running_time(), print_fork_time() ;
extern void print_usage() ;

extern void clear_radiosity(), clear_patch_radiosity() ;

extern void exit() ;


#endif



struct {
    char pad1[PAGE_SIZE];	 	/* padding to avoid false-sharing 
                                   and allow page-placement */	
    int n_local_free_task ;
    Task *local_free_task ;
    int crnt_taskq_id ;
    char pad2[PAGE_SIZE];	 	/* padding to avoid false-sharing 
                                   and allow page-placement */	
}  task_struct[MAX_PROCESSORS];

/***************************************************************************
 ****************************************************************************
 *
 *    Methods for Task object
 *
 ****************************************************************************
 ****************************************************************************/
/***************************************************************************
 *
 *    process_tasks()
 *
 *    Process tasks in the task queue. Task type is specified by the mask.
 *    Multiple task types may be specified by bit-oring the task type.
 *
 ****************************************************************************/

#define QUEUES_VISITED (n_taskqueues)
#define DEQUEUE_TASK(q,v,p) (dequeue_task((q),(v),p))

void process_tasks(process_id)
  unsigned process_id;
{
    Task *t ;
    
    t = DEQUEUE_TASK( taskqueue_id[process_id], QUEUES_VISITED, process_id ) ;

    printf("%d process_tasks - A, t = %lx\n", process_id, t);
    
 retry_entry:
    while( t )
        {
            switch( t->task_type )
                {
                case TASK_MODELING:
                    process_model( t->task.model.model, t->task.model.type, process_id ) ;
                    break ;
                case TASK_BSP:
                    define_patch( t->task.bsp.patch, t->task.bsp.parent, process_id ) ;
                    break ;
                case TASK_FF_REFINEMENT:
                    ff_refine_elements( t->task.ref.e1, t->task.ref.e2, 0, process_id ) ;
                    break ;
                case TASK_RAY:
                    process_rays( t->task.ray.e, process_id, process_id ) ;
                    break ;
                case TASK_VISIBILITY:
                    visibility_task( t->task.vis.e, t->task.vis.inter,
                                    t->task.vis.n_inter, t->task.vis.k, process_id ) ;
                    break ;
                case TASK_RAD_AVERAGE:
                    radiosity_averaging( t->task.rad.e, t->task.rad.mode, process_id ) ;
                    break ;
                default:
                    fprintf( stderr, "Panic:process_tasks:Illegal task type\n" );
                }
            
            /* Free the task */
            free_task( t, process_id ) ;
            
            /* Get next task */
            t = DEQUEUE_TASK( taskqueue_id[process_id], QUEUES_VISITED, process_id ) ;
        }
    printf("%d process_tasks - B\n", process_id);
    
    
    /* Barrier. While waiting for other processors to finish, poll the task
       queues and resume processing if there is any task */
    
    {pthread_mutex_lock(&(global->pbar_lock));};
    /* Reset the barrier counter if not initialized */
    if( global->pbar_count >= n_processors )
        global->pbar_count = 0 ;
    
    /* Increment the counter */
    global->pbar_count++ ;
    {pthread_mutex_unlock(&(global->pbar_lock));};
    
    printf("%d process_tasks - C\n", process_id);
    /* barrier spin-wait loop */
    while( global->pbar_count < n_processors )
        {
            /* Wait for a while and then retry dequeue */
            if( _process_task_wait_loop(process_id) )
                break ;
            
            /* Waited for a while but other processors are still running.
               Poll the task queue again */
            t = DEQUEUE_TASK( taskqueue_id[process_id], QUEUES_VISITED, process_id ) ;
            if( t ) 
                {
                    /* Task found. Exit the barrier and work on it */
                    {pthread_mutex_lock(&(global->pbar_lock));};
                    global->pbar_count-- ;
                    {pthread_mutex_unlock(&(global->pbar_lock));};
                    goto retry_entry ;
                }
            // we need this because the gcc might optimize the read for a vairable so that the changes are not visible to another core!
            __sync_synchronize();
            
        }
    
    printf("%d process_tasks - D\n", process_id);
    {
	unsigned long	Error, Cycle;
	int		Cancel, Temp;

	Error = pthread_mutex_lock(&(global->barrier).mutex);
	if (Error != 0) {
		printf("Error while trying to get lock in barrier.\n");
		exit(-1);
	}

	Cycle = (global->barrier).cycle;
	if (++(global->barrier).counter != (n_processors)) {
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &Cancel);
		while (Cycle == (global->barrier).cycle) {
			Error = pthread_cond_wait(&(global->barrier).cv, &(global->barrier).mutex);
			if (Error != 0) {
				break;
			}
		}
		pthread_setcancelstate(Cancel, &Temp);
	} else {
		(global->barrier).cycle = !(global->barrier).cycle;
		(global->barrier).counter = 0;
		Error = pthread_cond_broadcast(&(global->barrier).cv);
	}
	pthread_mutex_unlock(&(global->barrier).mutex);
};
}


int _process_task_wait_loop(process_id)
  unsigned process_id;
{
    int  i ;
    int finished = 0 ;
    
    /* Wait for a while and then retry */
    for( i = 0 ; i < 1000 && ! finished ; i++ )
        {
            if(    (i & 0xff) == 0
               && (volatile)global->pbar_count >= n_processors )
                
                finished = 1 ;
        }
    
    return( finished ) ;
}



/***************************************************************************
 *
 *    create_modeling_task()
 *    create_bsp_task()
 *    create_ff_refine_task()
 *    create_ray_task()
 *    create_visibility_task()
 *    create_radavg_task()
 *
 ****************************************************************************/


void create_modeling_task( model, type, process_id )
  
  Model *model ;
  int   type ;
  unsigned process_id;
{
    /* Implemented this way (routine just calls another routine)
       for historical reasons */
    
    printf("In %s\n", __FUNCTION__);
    fflush(stdout);
    process_model( model, type, process_id ) ;
    return ;
}


void create_bsp_task( patch, parent, process_id )
  
  Patch *patch ;
  Patch *parent ;
  unsigned process_id;
{
    Task *t ;
    
    /* Implemented this way (routine just calls another routine)
       for historical reasons */
    
    define_patch( patch, parent, process_id ) ;
    return ;
}



void create_ff_refine_task( e1, e2, level, process_id)
  unsigned process_id;
  
  Element *e1, *e2 ;
  int level ;
{
    Task *t ;
    
    /* Check existing parallelism */
    if( taskq_too_long(&global->task_queue[ taskqueue_id[process_id] ]) )
        {
            /* Task queue is too long. Solve it immediately */
            ff_refine_elements( e1, e2, level, process_id ) ;
            return ;
        }
    
    /* Create a task */
    t = get_task(process_id) ;
    t->task_type = TASK_FF_REFINEMENT ;
    t->task.ref.e1              = e1 ;
    t->task.ref.e2              = e2 ;
    t->task.ref.level           = level ;
    
    /* Put in the queue */
    enqueue_task( taskqueue_id[process_id], t, TASK_INSERT, process_id ) ;
}




void create_ray_task( e, process_id )
  
  Element *e ;
  unsigned process_id;
{
    void enqueue_ray_task() ;
    
    /* Check existing parallelism */
    if(   ((e->n_interactions + e->n_vis_undef_inter)
           < N_inter_parallel_bf_refine)
       || taskq_too_long(&global->task_queue[ taskqueue_id[process_id] ]) )
        {
            /* Task size is small, or the queue is too long.
               Solve it immediately */
            process_rays( e, process_id ) ;
            return ;
        }
    
    /* Put in the queue */
    enqueue_ray_task( taskqueue_id[process_id], e, TASK_INSERT, process_id ) ;
}


void enqueue_ray_task( qid, e, mode, process_id )
  
  int qid ;
  Element *e ;
  int mode ;
  unsigned process_id;
{
    Task *t ;
    
    /* Create task object */
    t = get_task(process_id) ;
    t->task_type = TASK_RAY ;
    t->task.ray.e     = e ;
    
    /* Put in the queue */
    enqueue_task( qid, t, mode, process_id ) ;
}


void create_visibility_tasks( e, k, process_id )
  
  Element *e ;
  void (*k)() ;
  unsigned process_id;
{
    int n_tasks ;
    int remainder ;			     /* Residue of MOD(total_undefs)*/
    int i_cnt ;
    Interaction *top, *tail ;
    Task *t ;
    int total_undefs = 0 ;
    int tasks_created = 0 ;
    
    /* Check number of hard problems */
    for( top = e->vis_undef_inter ; top ; top = top->next )
        if( top->visibility == VISIBILITY_UNDEF )
            total_undefs++ ;
    
    if( total_undefs == 0 )
        {
            /* No process needs to be created. Call the continuation
               immediately */
            (*k)( e, process_id ) ;
            return ;
        }
    
    /* Check existing parallelism */
    if(   (total_undefs < N_visibility_per_task)
       || taskq_too_long(&global->task_queue[ taskqueue_id[process_id] ]) )
        {
            /* Task size is small, or the queue is too long.
               Solve it immediately. */
            visibility_task( e, e->vis_undef_inter,
                            e->n_vis_undef_inter, k, process_id ) ;
            
            return ;
        }
    
    /* Create multiple tasks. Hard problems (i.e. where visibility comp is
       really necessary) are divided into 'n_tasks' groups by residue
       number division (or Bresenham's DDA) */
    /* Note: once the first task is enqueued, the vis-undef list may be
       modified while other tasks are being created. So, any information
       that is necessary in the for-loop must be read from the element
       and saved locally */
    
    n_tasks = (total_undefs + N_visibility_per_task - 1)
        / N_visibility_per_task ;
    remainder = 0 ;
    i_cnt = 0 ;
    for( top = e->vis_undef_inter, tail = top ; tail ; tail = tail->next )
        {
            i_cnt++ ;
            
            if( tail->visibility != VISIBILITY_UNDEF )
                continue ;
            
            remainder += n_tasks ;
            
            if( remainder >= total_undefs )
                {
                    /* Create a task */
                    
                    /* For the last task, append following (easy) interactions
                       if there is any */
                    tasks_created++ ;
                    if( tasks_created >= n_tasks )
                        for( ; tail->next ; tail = tail->next, i_cnt++ ) ;
                    
                    /* Set task descriptor */
                    t = get_task(process_id) ;
                    t->task_type = TASK_VISIBILITY ;
                    t->task.vis.e       = e ;
                    t->task.vis.inter   = top ;
                    t->task.vis.n_inter = i_cnt ;
                    t->task.vis.k       = k ;
                    
                    /* Enqueue */
                    enqueue_task( taskqueue_id[process_id], t, TASK_INSERT, process_id ) ;
                    
                    /* Update pointer and the residue variable */
                    top = tail->next ;
                    remainder -= total_undefs ;
                    i_cnt = 0 ;
                }
        }
}




void create_radavg_task( e, mode, process_id )
  
  Element *e ;
  int mode ;
  unsigned process_id;
{
    void enqueue_radavg_task() ;
    
    /* Check existing parallelism */
    if(   (e->n_interactions < N_inter_parallel_bf_refine)
       || taskq_too_long(&global->task_queue[ taskqueue_id[process_id] ]) )
        {
            /* Task size is too small or queue is too long.
               Solve it immediately */
            radiosity_averaging( e, mode, process_id ) ;
            return ;
        }
    
    /* Put in the queue */
    enqueue_radavg_task( taskqueue_id[process_id], e, mode, process_id ) ;
}


void enqueue_radavg_task( qid, e, mode, process_id )
  
  int qid ;
  Element *e ;
  int mode ;
  unsigned process_id;
{
    Task *t ;
    
    /* Create task object */
    t = get_task(process_id) ;
    t->task_type = TASK_RAD_AVERAGE ;
    t->task.rad.e     = e ;
    t->task.rad.mode  = mode ;
    
    /* Put in the queue */
    enqueue_task( qid, t, TASK_INSERT, process_id ) ;
}



/***************************************************************************
 *
 *    enqueue_task()
 *    dequeue_task()
 *
 ****************************************************************************/

void enqueue_task( qid, task, mode, process_id )
  
  int  qid ;
  Task *task ;
  int  mode ;
{
    Task_Queue *tq ;
    
    
    tq = &global->task_queue[ qid ] ;
    
    /* Lock the task queue */
    {pthread_mutex_lock(&(tq->q_lock));};
    
    if( tq->tail == 0 )
        {
            /* The first task in the queue */
            tq->tail    = task ;
            tq->top     = task ;
            tq->n_tasks = 1 ;
        }
    else
        {
            /* Usual case */
            if( mode == TASK_APPEND )
                {
                    tq->tail->next = task ;
                    tq->tail = task ;
                    tq->n_tasks++ ;
                }
            else
                {
                    task->next = tq->top ;
                    tq->top = task ;
                    tq->n_tasks++ ;
                }
        }
    
    /* Unlock the task queue */
    {pthread_mutex_unlock(&(tq->q_lock));};
}




Task *dequeue_task( qid, max_visit, process_id )
  /*
   *    Attempts to dequeue first from the specified queue (qid), but if no 
   *	 task is found the routine searches max_visit other queues and returns
   *    a task. If a task is taken from another queue, the task is taken from
   *    the tail of the queue (usually, larger amount of work is involved than
   *    the task at the top of the queue and more locality can be exploited
   *  	 within the stolen task).
   */
  int qid ;
  int max_visit ;
  unsigned process_id;
{
    Task_Queue *tq ;
    Task *t = 0 ;
    Task *prev ;
    int visit_count = 0 ;
    int sign = -1 ;		      /* The first retry will go backward */
    int offset ;
    
    /* Check number of queues to be visited */
    if( max_visit > n_taskqueues )
        max_visit = n_taskqueues ;
    
    /* Get next task */
    while( visit_count < max_visit )
        {
            /* Select a task queue */
            tq = &global->task_queue[ qid ] ;
            
            /* Check the length (test-test&set) */
            if( tq->n_tasks > 0 )
                {
                    /* Lock the task queue */
                    {pthread_mutex_lock(&(tq->q_lock));};
                    if( tq->top )
                        {
                            if( qid == taskqueue_id[process_id] )
                                {
                                    t = tq->top ;
                                    tq->top = t->next ;
                                    if( tq->top == 0 )
                                        tq->tail = 0 ;
                                    tq->n_tasks-- ;
                                }
                            else
                                {
                                    /* Get tail */
                                    for( prev = 0, t = tq->top ; t->next ;
                                        prev = t, t = t->next ) ;
                                    
                                    if( prev == 0 )
                                        tq->top = 0 ;
                                    else
                                        prev->next = 0 ;
                                    tq->tail = prev ;
                                    tq->n_tasks-- ;
                                }
                        }
                    /* Unlock the task queue */
                    {pthread_mutex_unlock(&(tq->q_lock));};
                    break ;
                }
            
            /* Update visit count */
            visit_count++ ;
            
            /* Compute next taskqueue ID */
            offset = (sign > 0)? visit_count : -visit_count ;
            sign = -sign ;
            
            qid += offset ;
            if( qid < 0 )
                qid += n_taskqueues ;
            else if( qid >= n_taskqueues )
                qid -= n_taskqueues ;
        }
    
    return( t ) ;
}


/***************************************************************************
 *
 *    get_task()    Create a new instance of Task
 *    free_task()   Free a Task object
 *
 ****************************************************************************/


Task *get_task(process_id)
  unsigned process_id;
{
    Task *p ;
    Task_Queue *tq ;
    int i ;
    int q_id ;
    int retry_count = 0 ;
    
    /* First, check local task queue */
    if( task_struct[process_id].local_free_task == 0 )
        {
            /* If empty, allocate task objects from the shared list */
            q_id = taskqueue_id[process_id] ;
            
            while( task_struct[process_id].local_free_task == 0 )
                {
                    tq = &global->task_queue[ q_id ] ;
                    
                    if( tq->n_free > 0 )
                        {
                            {pthread_mutex_lock(&(tq->f_lock));};
                            if( tq->free )
                                {
                                    /* Scan the free list */
                                    for( i = 1, p = tq->free ;
                                        (i < N_ALLOCATE_LOCAL_TASK) && p->next ;
                                        i++, p = p->next ) ;
                                    
                                    task_struct[process_id].local_free_task = tq->free ;
                                    task_struct[process_id].n_local_free_task = i ;
                                    tq->free = p->next ;
                                    tq->n_free -= i ;
                                    p->next = 0 ;
                                    {pthread_mutex_unlock(&(tq->f_lock));};
                                    break ;
                                }
                            {pthread_mutex_unlock(&(tq->f_lock));};
                        }
                    
                    /* Try next task queue */
                    if( ++q_id >= n_taskqueues )
                        q_id = 0 ;
                    
                    /* Check retry count */
                    if( ++retry_count > MAX_TASKGET_RETRY )
                        {
                            fprintf( stderr, "Panic(P%d):No free task\n",
                                    process_id ) ;
                            fprintf( stderr, "  Local %d\n", task_struct[process_id].n_local_free_task ) ;
                            fprintf( stderr, "  Q0 free %d\n",
                                    global->task_queue[0].n_free ) ;
                            fprintf( stderr, "  Q0 task %d\n",
                                    global->task_queue[0].n_tasks ) ;
                            exit(1) ;
                        }
                }
        }
    
    
    /* Delete from the queue */
    p = task_struct[process_id].local_free_task ;
    task_struct[process_id].local_free_task = p->next ;
    task_struct[process_id].n_local_free_task-- ;
    
    /* Clear pointer just in case.. */
    p->next = 0 ;
    
    
    return( p ) ;
}



void free_task( task, process_id )
  
  Task *task ;
  unsigned process_id;
{
    Task_Queue *tq ;
    Task *p, *top ;
    int i ;
    
    /* Insert to the local queue */
    task->next = task_struct[process_id].local_free_task ;
    task_struct[process_id].local_free_task = task ;
    task_struct[process_id].n_local_free_task++ ;
    
    /* If local list is too long, export some tasks */
    if( task_struct[process_id].n_local_free_task >= (N_ALLOCATE_LOCAL_TASK * 2) )
        {
            tq = &global->task_queue[ taskqueue_id[process_id] ] ;
            
            for( i = 1, p = task_struct[process_id].local_free_task ;
                i < N_ALLOCATE_LOCAL_TASK ;   i++, p = p->next ) ;
            
            /* Update local list */
            top = task_struct[process_id].local_free_task ;
            task_struct[process_id].local_free_task = p->next ;
            task_struct[process_id].n_local_free_task -= i ;
            
            /* Insert in the shared list */
            {pthread_mutex_lock(&(tq->f_lock));};
            p->next = tq->free ;
            tq->free = top ;
            tq->n_free += i ;
            {pthread_mutex_unlock(&(tq->f_lock));};
        }
}



/***************************************************************************
 *
 *    init_taskq()
 *
 *    Initialize task free list and the task queue.
 *    This routine must be called when only one process is active.
 *
 ****************************************************************************/


void init_taskq(process_id)
  unsigned process_id;
{
    int i ;
    int qid ;
    int task_index = 0 ;
    int task_per_queue ;
    int n_tasks ;
    
    /* Reset task assignment index */
    task_struct[process_id].crnt_taskq_id = 0 ;
    
    /* Initialize task queues */
    task_per_queue = (MAX_TASKS + n_taskqueues - 1) / n_taskqueues ;
    
    for( qid = 0 ; qid < n_taskqueues ; qid++ )
        {
            /* Initialize free list */
            if (task_index + task_per_queue > MAX_TASKS )
                n_tasks = MAX_TASKS - task_index ;
            else
                n_tasks = task_per_queue ;
            
            for( i = task_index ; i < task_index + n_tasks - 1 ; i++ )
                global->task_buf[i].next = &global->task_buf[i+1] ;
            global->task_buf[ i ].next = 0 ;
            
            global->task_queue[ qid ].free = &global->task_buf[ task_index ] ;
            global->task_queue[ qid ].n_free = n_tasks ;
            
            /* Initialize task queue */
            global->task_queue[ qid ].top     = 0 ;
            global->task_queue[ qid ].tail    = 0 ;
            global->task_queue[ qid ].n_tasks = 0 ;
            
            /* Initialize locks */
            {pthread_mutex_init(&(global->task_queue[ qid ].q_lock), NULL);};
            {pthread_mutex_init(&(global->task_queue[ qid ].f_lock), NULL);};
            
            /* Update index for next queue */
            task_index += n_tasks ;
        }
    
    /* Initialize local free lists */
    task_struct[process_id].n_local_free_task      = 0 ;
    task_struct[process_id].local_free_task        = 0 ;
}



/***************************************************************************
 *
 *    check_task_counter()
 *
 *    Check task counter and return TRUE if this is the first task.
 *
 ****************************************************************************/

int check_task_counter(process_id)
  unsigned process_id;
{
    int flag = 0 ;
    
    
    {pthread_mutex_lock(&(global->task_counter_lock));};
    
    if( global->task_counter == 0 )
        /* First processor */
        flag = 1 ;
    
    global->task_counter++ ;
    if( global->task_counter >= n_processors )
        global->task_counter = 0 ;
    
    {pthread_mutex_unlock(&(global->task_counter_lock));};
    
    return( flag ) ;
}



/***************************************************************************
 *
 *    assign_taskq()
 *
 *    Assign process its task queue.
 *
 ****************************************************************************/

int assign_taskq(process_id)
  unsigned process_id;
{
    int qid ;
    
    qid = task_struct[process_id].crnt_taskq_id++ ;
    
    if( task_struct[process_id].crnt_taskq_id >= n_taskqueues )
        task_struct[process_id].crnt_taskq_id = 0 ;
    
    return( qid ) ;
}


/***************************************************************************
 *
 *    print_task()
 *    print_taskq()
 *
 *    Print contents of a task.
 *
 ****************************************************************************/

void print_task( task, process_id )
  
  Task *task ;
  unsigned process_id;
{
    if( task == 0 )
        {
            printf( "Task (NULL)\n" ) ;
            return ;
        }
    
    switch( task->task_type )
        {
        case TASK_MODELING:
            printf( "Task (Model)\n" ) ;
            break ;
        case TASK_BSP:
            printf( "Task (BSP)\n" ) ;
            break ;
        case TASK_FF_REFINEMENT:
            printf( "Task (FF Refinement)\n" ) ;
            break ;
        case TASK_RAY:
            printf( "Task (Ray)  (patch ID %d)\n",
                   task->task.ray.e->patch->seq_no ) ;
            break ;
        case TASK_VISIBILITY:
            printf( "Task (Visibility)  (patch ID %d)\n",
                   task->task.vis.e->patch->seq_no ) ;
            break ;
        case TASK_RAD_AVERAGE:
            printf( "Task (RadAvg)\n" ) ;
            break ;
        default:
            fprintf( stderr, "Task(Illegal task type %d)\n", task->task_type );
        }
}


void print_taskq( tq, process_id )
  
  Task_Queue *tq ;
  unsigned process_id;
{
    Task *t ;
    
    printf( "TaskQ: %d tasks in the queue\n", taskq_length(tq) ) ;
    for( t = taskq_top(tq) ; t ; t = t->next )
        {
            printf( "  " ) ;
            print_task( t, process_id ) ;
        }
}

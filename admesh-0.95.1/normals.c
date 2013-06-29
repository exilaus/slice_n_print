/*  ADMesh -- process triangulated solid meshes
 *  Copyright (C) 1995  Anthony D. Martin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 *  Questions, comments, suggestions, etc to <amartin@engr.csulb.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stl.h"

static void stl_reverse_facet(stl_file *stl, int facet_num);
/* static float stl_calculate_area(stl_facet *facet); */
static void stl_reverse_vector(float v[]);
int stl_check_normal_vector(stl_file *stl, int facet_num, int normal_fix_flag);

static void
stl_reverse_facet(stl_file *stl, int facet_num)
{
  stl_vertex tmp_vertex;
  /*  int tmp_neighbor;*/
  int neighbor[3];
  int vnot[3];

  stl->stats.facets_reversed += 1;
  
  neighbor[0] = stl->neighbors_start[facet_num].neighbor[0];
  neighbor[1] = stl->neighbors_start[facet_num].neighbor[1];
  neighbor[2] = stl->neighbors_start[facet_num].neighbor[2];
  vnot[0] = stl->neighbors_start[facet_num].which_vertex_not[0];
  vnot[1] = stl->neighbors_start[facet_num].which_vertex_not[1];
  vnot[2] = stl->neighbors_start[facet_num].which_vertex_not[2];

  /* reverse the facet */
  tmp_vertex = stl->facet_start[facet_num].vertex[0];
  stl->facet_start[facet_num].vertex[0] = 
    stl->facet_start[facet_num].vertex[1];
  stl->facet_start[facet_num].vertex[1] = tmp_vertex;

  /* fix the vnots of the neighboring facets */
  if(neighbor[0] != -1)
  stl->neighbors_start[neighbor[0]].which_vertex_not[(vnot[0] + 1) % 3] = 
    (stl->neighbors_start[neighbor[0]].
     which_vertex_not[(vnot[0] + 1) % 3] + 3) % 6;
  if(neighbor[1] != -1)
  stl->neighbors_start[neighbor[1]].which_vertex_not[(vnot[1] + 1) % 3] = 
    (stl->neighbors_start[neighbor[1]].
     which_vertex_not[(vnot[1] + 1) % 3] + 4) % 6;
  if(neighbor[2] != -1)
  stl->neighbors_start[neighbor[2]].which_vertex_not[(vnot[2] + 1) % 3] = 
    (stl->neighbors_start[neighbor[2]].
     which_vertex_not[(vnot[2] + 1) % 3] + 2) % 6;

  /* swap the neighbors of the facet that is being reversed */
  stl->neighbors_start[facet_num].neighbor[1] = neighbor[2];
  stl->neighbors_start[facet_num].neighbor[2] = neighbor[1];

  /* swap the vnots of the facet that is being reversed */
  stl->neighbors_start[facet_num].which_vertex_not[1] = vnot[2];
  stl->neighbors_start[facet_num].which_vertex_not[2] = vnot[1];

  /* reverse the values of the vnots of the facet that is being reversed */
  stl->neighbors_start[facet_num].which_vertex_not[0] =
    (stl->neighbors_start[facet_num].which_vertex_not[0] + 3) % 6;
  stl->neighbors_start[facet_num].which_vertex_not[1] =
    (stl->neighbors_start[facet_num].which_vertex_not[1] + 3) % 6;
  stl->neighbors_start[facet_num].which_vertex_not[2] =
    (stl->neighbors_start[facet_num].which_vertex_not[2] + 3) % 6;
}

void
stl_fix_normal_directions(stl_file *stl)
{
  char *norm_sw;
  /*  int edge_num;*/
  /*  int vnot;*/
  int checked = 0;
  int facet_num;
  /*  int next_facet;*/
  int i;
  int j;
  int checked_before = 0;
  struct stl_normal
  {
    int               facet_num;
    struct stl_normal *next;
  };
  struct stl_normal *head;
  struct stl_normal *tail;
  struct stl_normal *new;
  struct stl_normal *temp;
  
  
  /* Initialize linked list. */
  head = malloc(sizeof(struct stl_normal));
  if(head == NULL) perror("stl_fix_normal_directions");
  tail = malloc(sizeof(struct stl_normal));
  if(tail == NULL) perror("stl_fix_normal_directions");
  head->next = tail;
  tail->next = tail;

  /* Initialize list that keeps track of already fixed facets. */
  norm_sw = calloc(stl->stats.number_of_facets, sizeof(char));
  if(norm_sw == NULL) perror("stl_fix_normal_directions");
  

  facet_num = 0;
  if(stl_check_normal_vector(stl, 0, 0) == 2)
    stl_reverse_facet(stl, 0);
     
  norm_sw[facet_num] = 1;
  /*  edge_num = 0;
      vnot = stl->neighbors_start[0].which_vertex_not[0];
      */
  checked++;

  for(;;)
    {
      /* Add neighbors_to_list. */
      for(j = 0; j < 3; j++)
	{
	  /* Reverse the neighboring facets if necessary. */
	  if(stl->neighbors_start[facet_num].which_vertex_not[j] > 2)
	    {
	      if(stl->neighbors_start[facet_num].neighbor[j] != -1)
		{
		  stl_reverse_facet
		    (stl, stl->neighbors_start[facet_num].neighbor[j]);
		}
	    }
	  if(stl->neighbors_start[facet_num].neighbor[j] != -1)
	    {
	      if(norm_sw[stl->neighbors_start[facet_num].neighbor[j]] != 1)
		{
		  /* Add node to beginning of list. */
		  new = malloc(sizeof(struct stl_normal));
		  if(new == NULL) perror("stl_fix_normal_directions");
		  new->facet_num = stl->neighbors_start[facet_num].neighbor[j];
		  new->next = head->next;
		  head->next = new;
		}
	    }
	}
      /* Get next facet to fix from top of list. */
      if(head->next != tail)
	{
	  facet_num = head->next->facet_num;	  
	  if(norm_sw[facet_num] != 1) /* If facet is in list mutiple times */
	    {
	      norm_sw[facet_num] = 1; /* Record this one as being fixed. */
	      checked++;
	    }
	  temp = head->next;	/* Delete this facet from the list. */
	  head->next = head->next->next;
	  free(temp);
	}
      else
	{
	  /* All of the facets in this part have been fixed. */
	  stl->stats.number_of_parts += 1;
	  /* There are (checked-checked_before) facets */
	  /* in part stl->stats.number_of_parts */
	  checked_before = checked;
	  if(checked == stl->stats.number_of_facets)
	    {
	      /* All of the facets have been checked.  Bail out. */
	      break;
	    }
	  else
	    {
	      /* There is another part here.  Find it and continue. */
	      for(i = 0; i < stl->stats.number_of_facets; i++)
		{
		  if(norm_sw[i] == 0)
		    {	  /* This is the first facet of the next part. */
		      facet_num = i;
		      if(stl_check_normal_vector(stl, i, 0) == 2)
			{
			  stl_reverse_facet(stl, i);
			}
		      
		      norm_sw[facet_num] = 1;
		      checked++;
		      break;
		    }
		}
	    }
	}
    }
  free(head);
  free(tail);
  free(norm_sw);
}

int
stl_check_normal_vector(stl_file *stl, int facet_num, int normal_fix_flag)
{
  /* Returns 0 if the normal is within tolerance */
  /* Returns 1 if the normal is not within tolerance, but direction is OK */
  /* Returns 2 if the normal is not within tolerance and backwards */
  /* Returns 4 if the status is unknown. */
  
  float normal[3];
  float test_norm[3];
  stl_facet *facet;

  facet = &stl->facet_start[facet_num];

  stl_calculate_normal(normal, facet);
  stl_normalize_vector(normal);
  
  if(   (ABS(normal[0] - facet->normal.x) < 0.001)
     && (ABS(normal[1] - facet->normal.y) < 0.001)
     && (ABS(normal[2] - facet->normal.z) < 0.001))
    {
      /* It is not really necessary to change the values here */
      /* but just for consistency, I will. */
      facet->normal.x = normal[0];
      facet->normal.y = normal[1];
      facet->normal.z = normal[2];
      return 0;
    }
  
  test_norm[0] = facet->normal.x;
  test_norm[1] = facet->normal.y;
  test_norm[2] = facet->normal.z;
  
  stl_normalize_vector(test_norm);
  if(   (ABS(normal[0] - test_norm[0]) < 0.001)
     && (ABS(normal[1] - test_norm[1]) < 0.001)
     && (ABS(normal[2] - test_norm[2]) < 0.001))
    {
      if(normal_fix_flag)
	{
	  facet->normal.x = normal[0];
	  facet->normal.y = normal[1];
	  facet->normal.z = normal[2];
	  stl->stats.normals_fixed += 1;
	}
      return 1;
    }

  stl_reverse_vector(test_norm);
  if(   (ABS(normal[0] - test_norm[0]) < 0.001)
     && (ABS(normal[1] - test_norm[1]) < 0.001)
     && (ABS(normal[2] - test_norm[2]) < 0.001))
    {
      /* Facet is backwards. */
      if(normal_fix_flag)
	{
	  facet->normal.x = normal[0];
	  facet->normal.y = normal[1];
	  facet->normal.z = normal[2];
	  stl->stats.normals_fixed += 1;
	}
      return 2;
    }
  if(normal_fix_flag)
    {
      facet->normal.x = normal[0];
      facet->normal.y = normal[1];
      facet->normal.z = normal[2];
      stl->stats.normals_fixed += 1;
    } 
  return 4;
}

static void
stl_reverse_vector(float v[])
{
  v[0] *= -1;
  v[1] *= -1;
  v[2] *= -1;
}


void
stl_calculate_normal(float normal[], stl_facet *facet)
{
  float v1[3];
  float v2[3];

  v1[0] = facet->vertex[1].x - facet->vertex[0].x;
  v1[1] = facet->vertex[1].y - facet->vertex[0].y;
  v1[2] = facet->vertex[1].z - facet->vertex[0].z;
  v2[0] = facet->vertex[2].x - facet->vertex[0].x;
  v2[1] = facet->vertex[2].y - facet->vertex[0].y;
  v2[2] = facet->vertex[2].z - facet->vertex[0].z;

  normal[0] = (float)((double)v1[1] * (double)v2[2]) - ((double)v1[2] * (double)v2[1]);
  normal[1] = (float)((double)v1[2] * (double)v2[0]) - ((double)v1[0] * (double)v2[2]);
  normal[2] = (float)((double)v1[0] * (double)v2[1]) - ((double)v1[1] * (double)v2[0]);
}

/*
static float
stl_calculate_area(stl_facet *facet)
{
  float cross[3][3];
  float sum[3];
  float normal[3];
  float area;
  int i;
  
  for(i = 0; i < 3; i++)
    {
      cross[i][0] = ((facet->vertex[i].y * facet->vertex[(i + 1) % 3].z) -
		     (facet->vertex[i].z * facet->vertex[(i + 1) % 3].y));
      cross[i][1] = ((facet->vertex[i].z * facet->vertex[(i + 1) % 3].x) -
		     (facet->vertex[i].x * facet->vertex[(i + 1) % 3].z));
      cross[i][2] = ((facet->vertex[i].x * facet->vertex[(i + 1) % 3].y) -
		     (facet->vertex[i].y * facet->vertex[(i + 1) % 3].x));
    }
  
  sum[0] = cross[0][0] + cross[1][0] + cross[2][0];
  sum[1] = cross[0][1] + cross[1][1] + cross[2][1];
  sum[2] = cross[0][2] + cross[1][2] + cross[2][2];
  
  stl_calculate_normal(normal, facet);
  stl_normalize_vector(normal);
  area = 0.5 * (normal[0] * sum[0] + normal[1] * sum[1] +
		normal[2] * sum[2]);
  return ABS(area);
}
*/

void stl_normalize_vector(float v[])
{
  double length;
  double factor;
  float min_normal_length;
  
  length = sqrt((double)v[0] * (double)v[0] + (double)v[1] * (double)v[1] + (double)v[2] * (double)v[2]);
  min_normal_length = 0.000000000001;
  if(length < min_normal_length)
    {
      v[0] = 1.0;
      v[1] = 0.0;
      v[2] = 0.0;
      return;
    }  
  factor = 1.0 / length;
  v[0] *= factor;
  v[1] *= factor;
  v[2] *= factor;
}

void
stl_fix_normal_values(stl_file *stl)
{
  int i;
  
  for(i = 0; i < stl->stats.number_of_facets; i++)
    {
      stl_check_normal_vector(stl, i, 1);
    }
}

void
stl_reverse_all_facets(stl_file *stl)
{
  int i;
  float normal[3];
  
  for(i = 0; i < stl->stats.number_of_facets; i++)
    {
      stl_reverse_facet(stl, i);
      stl_calculate_normal(normal, &stl->facet_start[i]);
      stl_normalize_vector(normal);
      stl->facet_start[i].normal.x = normal[0];
      stl->facet_start[i].normal.y = normal[1];
      stl->facet_start[i].normal.z = normal[2];
    }
}


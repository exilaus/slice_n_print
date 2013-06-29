/*  ADMesh -- process triangulated solid meshes
 *  Copyright (C) 1995, 1996  Anthony D. Martin
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

#include <stdlib.h>

#include "stl.h"


void
stl_generate_shared_vertices(stl_file *stl)
{
  int i;
  int j;
  int first_facet;
  int direction;
  int facet_num;
  int vnot;
  int next_edge;
  int pivot_vertex;
  int next_facet;
  int reversed;
  
  stl->v_indices = 
    calloc(stl->stats.number_of_facets, sizeof(v_indices_struct));
  if(stl->v_indices == NULL) perror("stl_generate_shared_vertices");
  stl->v_shared = 
    calloc((stl->stats.number_of_facets / 2), sizeof(stl_vertex));
  if(stl->v_shared == NULL) perror("stl_generate_shared_vertices");
  stl->stats.shared_malloced = stl->stats.number_of_facets / 2;
  stl->stats.shared_vertices = 0;
  
  for(i = 0; i < stl->stats.number_of_facets; i++)
    {
      stl->v_indices[i].vertex[0] = -1;
      stl->v_indices[i].vertex[1] = -1;
      stl->v_indices[i].vertex[2] = -1;
    }
  

  for(i = 0; i < stl->stats.number_of_facets; i++)
    {
      first_facet = i;
      for(j = 0; j < 3; j++)
	{
	  if(stl->v_indices[i].vertex[j] != -1)
	    {
	      continue;
	    }
	  if(stl->stats.shared_vertices == stl->stats.shared_malloced)
	    {
	      stl->stats.shared_malloced += 1024;
	      stl->v_shared = realloc(stl->v_shared, 
			   stl->stats.shared_malloced * sizeof(stl_vertex));
	      if(stl->v_shared == NULL) perror("stl_generate_shared_vertices");
	    }
	      
	  stl->v_shared[stl->stats.shared_vertices] = 
	    stl->facet_start[i].vertex[j];

	  direction = 0;
	  reversed = 0;
	  facet_num = i;
	  vnot = (j + 2) % 3;

	  for(;;)
	    {
	      if(vnot > 2)
		{
		  if(direction == 0)
		    {
		      pivot_vertex = (vnot + 2) % 3;
		      next_edge = pivot_vertex;
		      direction = 1;
		    }
		  else
		    {
		      pivot_vertex = (vnot + 1) % 3;
		      next_edge = vnot % 3;
		      direction = 0;
		    }
		}
	      else
		{
		  if(direction == 0)
		    {
		      pivot_vertex = (vnot + 1) % 3;
		      next_edge = vnot;
		    }
		  else
		    {
		      pivot_vertex = (vnot + 2) % 3;
		      next_edge = pivot_vertex;
		    }
		}
	      stl->v_indices[facet_num].vertex[pivot_vertex] =
		stl->stats.shared_vertices;
	      
	      next_facet = stl->neighbors_start[facet_num].neighbor[next_edge];
	      if(next_facet == -1)
		{
		  if(reversed)
		    {
		      break;
		    }
		  else
		    {
		      direction = 1;
		      vnot = (j + 1) % 3;
		      reversed = 1;
		      facet_num = first_facet;
		    }
		}
	      else if(next_facet != first_facet)
		{
		  vnot = stl->neighbors_start[facet_num].
		    which_vertex_not[next_edge];
		  facet_num = next_facet;
		}
	      else
		{
		  break;
		}
	    }
	  stl->stats.shared_vertices += 1;
	}
    }
}

void
stl_write_off(stl_file *stl, char *file)
{
  int i;
  FILE      *fp;
  char      *error_msg;
  
  
  /* Open the file */
  fp = fopen(file, "w");
  if(fp == NULL)
    {
      error_msg = 
	malloc(81 + strlen(file)); /* Allow 80 chars+file size for message */
      sprintf(error_msg, "stl_write_ascii: Couldn't open %s for writing",
	      file);
      perror(error_msg);
      free(error_msg);
      exit(1);
    }
  
  fprintf(fp, "OFF\n");
  fprintf(fp, "%d %d 0\n",
	  stl->stats.shared_vertices, stl->stats.number_of_facets);

  for(i = 0; i < stl->stats.shared_vertices; i++)
    {
      fprintf(fp, "\t%f %f %f\n",
	      stl->v_shared[i].x, stl->v_shared[i].y, stl->v_shared[i].z);
    }
  for(i = 0; i < stl->stats.number_of_facets; i++)
    {
      fprintf(fp, "\t3 %d %d %d\n", stl->v_indices[i].vertex[0],
	      stl->v_indices[i].vertex[1], stl->v_indices[i].vertex[2]);
    }
  fclose(fp);
}

void
stl_write_vrml(stl_file *stl, char *file)
{
  int i;
  FILE      *fp;
  char      *error_msg;
  
  
  /* Open the file */
  fp = fopen(file, "w");
  if(fp == NULL)
    {
      error_msg = 
	malloc(81 + strlen(file)); /* Allow 80 chars+file size for message */
      sprintf(error_msg, "stl_write_ascii: Couldn't open %s for writing",
	      file);
      perror(error_msg);
      free(error_msg);
      exit(1);
    }
  
  fprintf(fp, "#VRML V1.0 ascii\n\n");
  fprintf(fp, "Separator {\n");
  fprintf(fp, "\tDEF STLShape ShapeHints {\n");
  fprintf(fp, "\t\tvertexOrdering COUNTERCLOCKWISE\n");
  fprintf(fp, "\t\tfaceType CONVEX\n");
  fprintf(fp, "\t\tshapeType SOLID\n");
  fprintf(fp, "\t\tcreaseAngle 0.0\n");
  fprintf(fp, "\t}\n");
  fprintf(fp, "\tDEF STLModel Separator {\n");
  fprintf(fp, "\t\tDEF STLColor Material {\n");
  fprintf(fp, "\t\t\temissiveColor 0.700000 0.700000 0.000000\n");
  fprintf(fp, "\t\t}\n");
  fprintf(fp, "\t\tDEF STLVertices Coordinate3 {\n");
  fprintf(fp, "\t\t\tpoint [\n");

  for(i = 0; i < (stl->stats.shared_vertices - 1); i++)
    {
      fprintf(fp, "\t\t\t\t%f %f %f,\n",
	      stl->v_shared[i].x, stl->v_shared[i].y, stl->v_shared[i].z);
    }
  fprintf(fp, "\t\t\t\t%f %f %f]\n",
	  stl->v_shared[i].x, stl->v_shared[i].y, stl->v_shared[i].z);
  fprintf(fp, "\t\t}\n");
  fprintf(fp, "\t\tDEF STLTriangles IndexedFaceSet {\n");
  fprintf(fp, "\t\t\tcoordIndex [\n");

  for(i = 0; i < (stl->stats.number_of_facets - 1); i++)
    {
      fprintf(fp, "\t\t\t\t%d, %d, %d, -1,\n", stl->v_indices[i].vertex[0],
	      stl->v_indices[i].vertex[1], stl->v_indices[i].vertex[2]);
    }
  fprintf(fp, "\t\t\t\t%d, %d, %d, -1]\n", stl->v_indices[i].vertex[0],
	  stl->v_indices[i].vertex[1], stl->v_indices[i].vertex[2]);
  fprintf(fp, "\t\t}\n");
  fprintf(fp, "\t}\n");
  fprintf(fp, "}\n");
  fclose(fp);
}

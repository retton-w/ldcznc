/*
** pref.h
** Login : <frank@frank-desktop>
** Started on  Sat Jul 25 15:55:39 2009 frank
** $Id$
** 
** Copyright (C) 2009 frank
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef   	PREF_H_
#define   	PREF_H_

//#include <utillib.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xmlParser.h"

static int save_pref(XMLNode &root_node, 
                     const char *sec, const char *key, const char *val)
{
        XMLNode sec_node;
        XMLNode key_node;

        sec_node = root_node.getChildNodeWithAttribute("sec", "name", sec);
        if (sec_node.getName() == NULL) {
                XMLNode sec_node_new = root_node.addChild("sec");
                sec_node_new.addAttribute("name", sec);

                XMLNode key_node_new = sec_node_new.addChild("key");
                key_node_new.addAttribute("name",  key);
                key_node_new.addAttribute("value", val);

                return 0;
        }

        key_node = sec_node.getChildNodeWithAttribute("key", "name", key);
        if (key_node.getName() == NULL) {
                XMLNode key_node_new = sec_node.addChild("key");
                key_node_new.addAttribute("name", key);
                key_node_new.addAttribute("value", val);

                return 0;
        }

        key_node.updateAttribute(val, NULL, "value");

        return 0;
}

static int load_pref(XMLNode &root_node, 
                     const char *sec, const char *key, const char *def, 
                     char *val)
{
        XMLNode sec_node;
        XMLNode key_node;

        sec_node = root_node.getChildNodeWithAttribute("sec", "name", sec);
        if (sec_node.getName() == NULL) {
                XMLNode sec_node_new = root_node.addChild("sec");
                sec_node_new.addAttribute("name", sec);

                XMLNode key_node_new = sec_node_new.addChild("key");
                key_node_new.addAttribute("name",  key);
                key_node_new.addAttribute("value", def);

                return -1;
        }

        key_node = sec_node.getChildNodeWithAttribute("key", "name", key);
        if (key_node.getName() == NULL) {
                XMLNode key_node_new = sec_node.addChild("key");
                key_node_new.addAttribute("name", key);
                key_node_new.addAttribute("value", def);

                return -1;
        }

        const char *attr = key_node.getAttribute("value");
        if (attr == NULL) {
                key_node.addAttribute("value", def);
                return -1;
        }

        strcpy(val, attr);

        return 0;
}

/*
 * class pref
 */
class pref {
public :
        enum PREF_TYPE {
                INT,        /* INT */
                INT_RANGE,  /* INT WITH RANGE */
                INT_VAR,    /* INT WITH STEP */
                STRING,     /* STRING */
                BOOL        /* BOOL */
        };
public :
        pref(const char *sec,
             const char *key,
             PREF_TYPE  type)
        {
                this->sec  = sec;
                this->key  = key;
                this->type = type;
        }
        virtual ~pref(){}
public :
        virtual int set(void *value) = 0;       /* set value */
        virtual int get(void *value) = 0;       /* get value */
        virtual int set_string(char *value) = 0;       /* set value */
        virtual int get_string(char *value) = 0;       /* get value */
        virtual int load(XMLNode &root_node) = 0; /* load from ini file */
        virtual int save(XMLNode &root_node) = 0; /* save to ini file */
protected :
        virtual int validate(void *value)
        {
                value = value;
                return 1;
        }
protected :
        const char *sec;
        const char *key;
        int  type;
};

/*
 * class pref_int
 */
class pref_int : public pref {
public :
pref_int(const char *sec,
                 const char *key,
                 int def) : pref(sec, key, INT)
        {
                this->cur = def;
                this->def = def;
        }
        ~pref_int(void)
        {
        }
public :
        int set(void *value)
        {
                this->cur = *(int *)value;
                return 0;
        }
        int get(void *value)
        {
                *(int *)value = this->cur;
                return 0;
        }
        int get_string(char *value)
        {
                int v;
                int ret = get((void *)&v);
                sprintf(value, "%d", v);
                return ret;
        }
        int set_string(char *value)
        {
                int v = atoi(value);
                return set((void *)&v);
        }
        int load(XMLNode &root_node)
        {
                char str_val[32];
                char str_def[32];
                int  ret = 0;
                sprintf(str_def, "%d", this->def);
                ret = load_pref(root_node, this->sec, this->key, str_def, str_val);
                if (ret < 0) {
                        this->cur = this->def;
                } else {
                        this->cur = atoi(str_val);
                }
                return 0;
        }
        int save(XMLNode &root_node)
        {
                char str_val[32];
                sprintf(str_val, "%d", this->cur);
                save_pref(root_node, this->sec, this->key, str_val);
                return 0;
        }
public :
        //protected :
        int cur;
        int def;
};

/*
 * class pref_int_range
 */
class pref_int_range : public pref_int {
public :
pref_int_range(const char *sec,
                     const char *key,
                     int def, int min, int max)
        : pref_int(sec, key, def)
        {
                this->type = INT_RANGE;
                this->cur  = def;
                this->def  = def;
                this->min  = min;
                this->max  = max;
        }
        ~pref_int_range(void){}
public :
        int set(void *value)
        {
                if (validate(value)) {
                        this->cur = *(int *)value;
                        return 0;
                } else {
                        return -1;
                }
        }
        int get(void *value)
        {
                if (validate(&(this->cur))) {
                        *(int *)value = this->cur;
                } else {
                        this->cur = this->def;
                        *(int *)value = this->cur;
                }
                return 0;
        }
        int load(XMLNode &root_node)
        {
                pref_int::load(root_node);
                if (!validate(&(this->cur))) {
                        this->cur = this->def;
                }
                return 0;
        }
        int save(XMLNode &root_node)
        {
                if (!validate(&(this->cur))) {
                        this->cur = this->def;
                }
                pref_int::save(root_node);
                return 0;
        }
protected :
        int validate(void *value) 
        {
                if ((*(int *)value > this->max) ||
                    (*(int *)value < this->min)) {
                        return 0;
                } else {
                        return 1;
                }
        }
protected :
        int min;
        int max;
};

/*
 * class pref_int_var
 */
class pref_int_var : public pref_int_range {
public :
pref_int_var(const char *sec,
             const char *key, int def, int min, int max,
             const char *step_key,
             int step_def, int step_min, int step_max)
        : pref_int_range(sec, key, def, min, max)
                ,step(sec, step_key, step_def, step_min, step_max)
        {
                this->type = INT_VAR;
        }
        ~pref_int_var(void){;}
public :
        int set_step(void *value) 
        {
                return step.set(value);
        }
        int get_setp(void *value)
        {
                return step.get(value);
        }
        int load(XMLNode &root_node)
        {
                pref_int_range::load(root_node);
                return step.load(root_node);
        }
        int save(XMLNode &root_node)
        {
                pref_int_range::save(root_node);
                return step.save(root_node);
        }
        int step_up(void)
        {
                int value = this->cur + step.cur;
                if (!validate(&value)) {
                        this->cur = value;
                        return 1;
                } else {
                        return 0;
                }
        }
        int step_down(void)
        {
                int value = this->cur - step.cur;
                if (!validate(&value)) {
                        this->cur = value;
                        return 1;
                } else {
                        return 0;
                }
        }
public :
        pref_int_range step;
};

/*
 * class pref_string
 */
class pref_string : public pref {
public :
        pref_string(const char *sec,
                          const char *key,
                          const char *def, int len)
                :pref(sec, key, STRING)
        {
                if ((int)strlen(def) > len) {
                        len = strlen(def) + 1;
                }
                this->cur = new char [len];
                this->def = new char [len];
                if ((this->cur != NULL) &&
                    (this->def != NULL)){
                        strcpy(this->cur, def);
                        strcpy(this->def, def);
                        this->len = len;
                } else {
                        this->len = 0;
                }
        }
        ~pref_string(void)
        {
                if (this->cur != NULL) {
                        delete [] this->cur;
                }
                if (this->def != NULL) {
                        delete [] this->def;
                }
        }
public :
        int set(void *value)
        {
                char *n   = *(char **)value;
                int  len  = strlen(n) + 1;
                char *str = NULL;
                if (len > this->len) {
                        str = new char [len];
                        if (str == NULL) {
                                return -1;
                        }
                        if (this->cur != NULL) {
                                delete [] this->cur;
                        }
                        this->cur = str;
                        this->len = len;
                }
                strcpy(this->cur, n);
                return 0;
        }
        int get(void *value)
        {
                *(char **)value = this->cur;
                if (this->cur == NULL) {
                        return -1;
                }
                return 0;
        }
        int get_string(char *value)
        {
                char *v;
                int ret;
                if ((ret = get((void *)&v)) == 0) {
                        strcpy(value, v);
                }
                return ret;
        }
        int set_string(char *value)
        {
                return set(&value);
        }
        int load(XMLNode &root_node)
        {
				char buf[256] = {0};
				char *str = NULL;
                int  ret = 0;
                ret = load_pref(root_node, this->sec, this->key, this->def, buf);
                if (ret < 0) {
                        strcpy(this->cur, this->def);
                        return 0;
                }

				int len = strlen(buf) + 1;
				if (len > this->len){
                    if (this->cur != NULL)
                    {
                        delete[] this->cur;
                        this->cur = NULL;
                    }

                    str = new char [len];
					this->cur = str;
                    this->len = len;
				}

				strcpy(this->cur, buf);

                return 0;
        }
        int save(XMLNode &root_node)
        {
                save_pref(root_node, this->sec, this->key, this->cur);
                return 0;
        }
protected :
        int validate(void *value)
        {
                value = value;
                return 1;
        }
protected :
        char *cur;
        char *def;
        int  len;
};

/*
 * class pref_bool
 */
class pref_bool : public pref_int {
public :
pref_bool(const char *sec,
          const char *key,
          int def)
        : pref_int(sec,key,def)
        {
                this->type = BOOL;
        }
        ~pref_bool(void){}
public :
        int set(void *value)
        {
                this->cur = *(int *)value;
                if (this->cur != 0) {
                        this->cur = 1;
                }
                return 0;
        }
        int get(void *value)
        {
                if (this->cur != 0) {
                        this->cur = 1;
                }
                *(int *)value = this->cur;
                return 0;
        }
        int load(XMLNode &root_node)
        {
                int ret = pref_int::load(root_node);
                if (this->cur != 0) {
                        this->cur = 1;
                }
                return ret;
        }
        int save(XMLNode &root_node)
        {
                if (this->cur != 0) {
                        this->cur = 1;
                }
                return pref_int::save(root_node);
        }
};

#endif 	    /* !PREF_H_ */


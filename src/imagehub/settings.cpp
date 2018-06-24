// settings.cpp
//
// DB-based settings.
//
//   (C) Copyright 2018 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <openssl/evp.h>

#include <QStringList>

#include "settings.h"

QString Settings::imageDirectory()
{
  return GetValue("IMAGE_DIRECTORY").toString();
}


QString Settings::filesystemDirectory()
{
  return GetValue("FILESYSTEM_DIRECTORY").toString();
}


QString Settings::cardDevice()
{
  return GetValue("CARD_DEVICE").toString();
}


QString Settings::imageFilename(int id)
{
  QString ret;

  QString sql=QString("select FILENAME from IMAGES where ")+
    QString().sprintf("ID=%d",id);
  SqlQuery *q=new SqlQuery(sql);
  if(q->first()) {
    ret=q->value(0).toString();
  }
  delete q;
  return ret;
}


QString Settings::imageFilename(const QString &pathname)
{
  QStringList f0=pathname.split("/",QString::SkipEmptyParts);
  return f0.last();
}


QString Settings::imagePathname(int id)
{
  return Settings::imageDirectory()+"/"+Settings::imageFilename(id);
}


QString Settings::imagePathname(const QString &filename)
{
  return Settings::imageDirectory()+"/"+filename;
}


QString Settings::sha1Hash(const QString &pathname)
{
  EVP_MD_CTX *mdctx;
  const EVP_MD *md;
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned md_len;
  int fd;
  char data[1024];
  ssize_t n;
  QString ret="";

  //
  // Open File
  //
  if((fd=open(pathname.toUtf8(),O_RDONLY))<0) {
    return QString();
  }

  //
  // Initialize Hash Generator
  //
  OpenSSL_add_all_digests();
  md=EVP_get_digestbyname("SHA1");
  if(!md) {
    printf("No such hash!\n");
  }
  mdctx=EVP_MD_CTX_create();
  EVP_DigestInit_ex(mdctx,md,NULL);

  //
  // Generate Hash
  //
  while((n=read(fd,data,1024))>0) {
    EVP_DigestUpdate(mdctx,data,n);
  }
  EVP_DigestFinal_ex(mdctx,md_value,&md_len);
  close(fd);

  //
  // Convert to String
  //
  for(unsigned i=0;i<md_len;i++) {
    ret+=QString().sprintf("%02x",md_value[i]);
  }

  EVP_cleanup();

  return ret;
}


bool Settings::fileCopy(const QString &destfile,const QString &srcfile,
			QString *err_msg)
{
  int src_fd;
  int dest_fd;
  ssize_t n;
  char data[1024];

  //
  // Open Files
  //
  if((src_fd=open(srcfile.toUtf8(),O_RDONLY))<0) {
    *err_msg=strerror(errno);
    return false;
  }
  if((dest_fd=open(destfile.toUtf8(),O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH))<0) {
    *err_msg=strerror(errno);
    close(src_fd);
    return false;
  }

  //
  // Copy
  //
  while((n=read(src_fd,data,1024))>0) {
    if(write(dest_fd,data,n)!=n) {
      *err_msg=strerror(errno);
      return false;
    }
  }

  //
  // Clean up
  //
  close(src_fd);
  close(dest_fd);
  *err_msg="ok";
  return true;
}


QVariant Settings::GetValue(const QString &field)
{
  QVariant ret;
  QString sql=QString("select ")+field+" from SETTINGS";
  SqlQuery *q=new SqlQuery(sql);
  if(q->first()) {
    ret=q->value(0);
  }
  return ret;
}

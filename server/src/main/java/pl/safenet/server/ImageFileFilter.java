package pl.safenet.server;

import java.io.*;


public class ImageFileFilter implements FileFilter
{
//  private final String[] acceptedExtensions = new String[] {".bmp", ".png", ".jpg", ".jpeg"};
  private final String[] acceptedExtensions = new String[] {".png"};

  @Override
  public boolean accept(File file)
  {
    for (String extension : acceptedExtensions)
    {
      if (file.getName().toLowerCase().endsWith(extension))
      {
        return true;
      }
    }
    return false;
  }
}

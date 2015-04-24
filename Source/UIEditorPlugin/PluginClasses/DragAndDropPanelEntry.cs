// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM		: ALL
// PRODUCT		: VISION
// VISIBILITY		: PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

using System;
using System.Collections.Generic;
using System.Text;

namespace UIEditorPlugin.Classes
{
  /// <summary>
  /// Helper class which encapsulates an entry for the panel list view
  /// </summary>
  public class DragAndDropPanelEntry : IComparable<DragAndDropPanelEntry>
  {
    public enum EntryType
    {
      Model = 1,
      StaticMesh = 2
    }

    #region Private Members

    EntryType _Type;

    string _Path;

    string _DisplayText;

    string _ThumbnailKey;

    #endregion

    public DragAndDropPanelEntry(EntryType Type, string Path, string DisplayText, string ThumbnailKey)
    {
      _Type = Type;
      _Path = Path;
      _DisplayText = DisplayText;
      _ThumbnailKey = ThumbnailKey;
    }

    #region Public Get Accessors

    public EntryType Type
    {
      get
      {
        return _Type;
      }
    }

    public string Path
    {
      get
      {
        return _Path;
      }
    }

    public string DisplayText
    {
      get
      {
        return _DisplayText;
      }
    }

    public string Thumbnailkey
    {
      get
      {
        return _ThumbnailKey;
      }
    }

    #endregion


    #region IComparable<EntityPanelEntry> Members

    public int CompareTo(DragAndDropPanelEntry other)
    {
      return _DisplayText.CompareTo(other.DisplayText);
    }

    #endregion
  }
}


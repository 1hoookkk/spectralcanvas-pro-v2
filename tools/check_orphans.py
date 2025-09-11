#!/usr/bin/env python3
"""
Ghost Code Prevention Tool - Orphan File Detector

Detects source files in the Source/ directory that are not explicitly
listed in CMakeLists.txt, preventing "ghost code" accumulation.

Usage:
    python tools/check_orphans.py

Exit codes:
    0 = Success (no orphans found)
    1 = Error (orphans found or other issue)
"""

import os
import re
import sys
from pathlib import Path


def find_cmake_sources(cmake_file: Path) -> set[str]:
    """Parse CMakeLists.txt to find all explicitly listed source files."""
    sources = set()
    
    if not cmake_file.exists():
        print(f"Warning: CMakeLists.txt not found at {cmake_file}")
        return sources
    
    try:
        content = cmake_file.read_text(encoding='utf-8')
        
        # Find all Source/ references in the CMake file
        # This regex looks for Source/ followed by path components ending in .cpp/.h
        pattern = r'Source/[\w/]+\.(?:cpp|h|hpp)'
        matches = re.findall(pattern, content, re.IGNORECASE)
        
        for match in matches:
            # Normalize path separators to forward slashes
            normalized = match.replace('\\', '/')
            sources.add(normalized)
            
    except Exception as e:
        print(f"Error reading CMakeLists.txt: {e}")
        
    return sources


def find_disk_sources(source_dir: Path) -> set[str]:
    """Walk the Source/ directory to find all .cpp/.h files on disk."""
    disk_files = set()
    
    if not source_dir.exists():
        print(f"Warning: Source directory not found at {source_dir}")
        return disk_files
    
    # Extensions to consider as source files
    source_extensions = {'.cpp', '.h', '.hpp', '.c', '.cc', '.cxx'}
    
    try:
        for file_path in source_dir.rglob('*'):
            if file_path.is_file() and file_path.suffix.lower() in source_extensions:
                # Skip experimental directories by convention
                if 'Experimental' in file_path.parts:
                    continue
                    
                # Convert to relative path from project root
                relative_path = file_path.relative_to(source_dir.parent)
                # Normalize path separators
                normalized = str(relative_path).replace('\\', '/')
                disk_files.add(normalized)
                
    except Exception as e:
        print(f"Error scanning source directory: {e}")
        
    return disk_files


def main():
    """Main script execution."""
    # Find project root (directory containing this script's parent)
    script_path = Path(__file__).resolve()
    project_root = script_path.parent.parent
    
    # Locate key project files
    cmake_path = project_root / 'CMakeLists.txt'
    source_path = project_root / 'Source'
    
    print("SpectralCanvasLite - Ghost Code Prevention")
    print("=" * 45)
    print(f"Project root: {project_root}")
    print(f"Checking: {cmake_path}")
    print(f"Source dir: {source_path}")
    print()
    
    # Find files in CMake and on disk
    cmake_sources = find_cmake_sources(cmake_path)
    disk_sources = find_disk_sources(source_path)
    
    print(f"Files in CMakeLists.txt: {len(cmake_sources)}")
    print(f"Files on disk: {len(disk_sources)}")
    
    # Find orphaned files (on disk but not in CMake)
    orphans = disk_sources - cmake_sources
    
    # Find missing files (in CMake but not on disk)
    missing = cmake_sources - disk_sources
    
    # Report results
    success = True
    
    if orphans:
        success = False
        print(f"\n❌ ORPHANED FILES ({len(orphans)} found):")
        print("   These files exist on disk but are not listed in CMakeLists.txt:")
        for orphan in sorted(orphans):
            print(f"     - {orphan}")
        print("\n   Fix: Add these files to the appropriate SC_*_SOURCES list in CMakeLists.txt")
        print("        or move them to an Experimental/ subdirectory if they're not ready.")
    
    if missing:
        success = False
        print(f"\n❌ MISSING FILES ({len(missing)} found):")
        print("   These files are listed in CMakeLists.txt but don't exist on disk:")
        for missing_file in sorted(missing):
            print(f"     - {missing_file}")
        print("\n   Fix: Remove these entries from CMakeLists.txt or create the missing files.")
    
    if success:
        print("✅ SUCCESS: No orphaned or missing files found.")
        print("   All source files are properly tracked in CMakeLists.txt.")
    
    # Debug info if requested
    if '--verbose' in sys.argv or '-v' in sys.argv:
        print(f"\nDEBUG INFO:")
        print("CMake sources:", sorted(cmake_sources))
        print("Disk sources:", sorted(disk_sources))
    
    print(f"\nGhost code prevention: {'PASSED' if success else 'FAILED'}")
    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main())
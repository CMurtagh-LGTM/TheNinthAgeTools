
import subprocess
import sys

from subprocess import PIPE

def run_process(cmd, check=True, input=None, *args, **kw):
  """Runs a subprocess returning the exit code.

  By default this function will raise an exception on failure.  Therefore this should only be
  used if you want to handle such failures.  For most subprocesses, failures are not recoverable
  and should be fatal.  In those cases the `check_call` wrapper should be preferred.
  """

  # Flush standard streams otherwise the output of the subprocess may appear in the
  # output before messages that we have already written.
  sys.stdout.flush()
  sys.stderr.flush()
  kw.setdefault('text', True)
  kw.setdefault('encoding', 'utf-8')
  ret = subprocess.run(cmd, check=check, input=input, *args, **kw)
  return ret


def normalize_path(path):
  """Normalize path separators to UNIX-style forward slashes.

  This can be useful when converting paths to URLs or JS strings,
  or when trying to generate consistent output file contents
  across all platforms.  In most cases UNIX-style separators work
  fine on windows.
  """
  return path.replace('\\', '/').replace('//', '/')


#if something
  #if somethingelse
    doThis();
    #if defined(SOMEPERIPHERAL)
      SOMEPERIPHERAL.SFR = foo;
    #elif defined(SOMEOTHERPERIPHERAL)
      if (SYSCFG.REVID > 0x08) {
SOMEOTHERPERIPHERAL.SFR=getValueForOther(foo);
      }
    #else
      return -1;
    #endif
  #endif
  return DoThatThing();
#endif

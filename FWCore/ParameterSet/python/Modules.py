from Mixins import _ConfigureComponent
from Mixins import _Unlabelable, _Labelable
from Mixins import _TypedParameterizable, PrintOptions
from SequenceTypes import _Sequenceable

from ExceptionHandling import *
class Service(_ConfigureComponent,_TypedParameterizable,_Unlabelable):
    def __init__(self,type_,*arg,**kargs):
        super(Service,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        proc._placeService(self.type_(),self)
    def insertInto(self, processDesc):
        newpset = processDesc.newPSet()
        newpset.addString(True, "@service_type", self.type_())
        self.insertContentsInto(newpset)
        processDesc.addService(newpset)


class ESSource(_ConfigureComponent,_TypedParameterizable,_Unlabelable,_Labelable):
    def __init__(self,type_,*arg,**kargs):
        super(ESSource,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        if name == '':
            name=self.type_()
        proc._placeESSource(name,self)
    def moduleLabel_(self,myname):
       result = myname
       if self.type_() == myname:
           result = ""
       return result
    def nameInProcessDesc_(self, myname):
       result = self.type_() + "@" + self.moduleLabel_(myname)
       return result


class ESProducer(_ConfigureComponent,_TypedParameterizable,_Unlabelable,_Labelable):
    def __init__(self,type_,*arg,**kargs):
        super(ESProducer,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        if name == '':
            name=self.type_()
        proc._placeESProducer(name,self)
    def moduleLabel_(self,myname):
       result = myname
       if self.type_() == myname:
           result = ''
       return result
    def nameInProcessDesc_(self, myname):
       result = self.type_() + "@" + self.moduleLabel_(myname)
       return result


class ESPrefer(_ConfigureComponent,_TypedParameterizable,_Unlabelable,_Labelable):
    def __init__(self,type_,*arg,**kargs):
        super(ESPrefer,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        if name == '':
            name=self.type_()
        proc._placeESPrefer(name,self)
    def moduleLabel_(self,myname):
       # the C++ parser can give it a name like "label@prefer".  Get rid of that.
       return myname.split('@')[0]
    def nameInProcessDesc_(self, myname):
       # the C++ parser can give it a name like "label@prefer".  Get rid of that.
       return "esprefer_" + self.type_() + "@" + myname.split('@')[0]
    def dumpPythonAs(self, label, options=PrintOptions()):
       name = self.moduleLabel_(label)
       if name == '':
          name = type_()
       return options.indentation()+'process.prefer(\"'+name+'\")'

class _Module(_ConfigureComponent,_TypedParameterizable,_Labelable,_Sequenceable):
    """base class for classes which denote framework event based 'modules'"""
    def __init__(self,type_,*arg,**kargs):
        super(_Module,self).__init__(type_,*arg,**kargs)
    def _clonesequence(self, lookuptable):
        try:
            return lookuptable[id(self)]
        except:
            # return something like "EDAnalyzer("foo", ...)"
            raise ModuleCloneError(self._errorstr())
    def _errorstr(self):
        typename = format_typename(self)
        return "%s('%s', ...)" %(typename, self.type_())

class EDProducer(_Module):
    def __init__(self,type_,*arg,**kargs):
        super(EDProducer,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        proc._placeProducer(name,self)


class EDFilter(_Module):
    def __init__(self,type_,*arg,**kargs):
        super(EDFilter,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        proc._placeFilter(name,self)


class EDAnalyzer(_Module):
    def __init__(self,type_,*arg,**kargs):
        super(EDAnalyzer,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        proc._placeAnalyzer(name,self)


class OutputModule(_Module):
    def __init__(self,type_,*arg,**kargs):
        super(OutputModule,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        proc._placeOutputModule(name,self)


class Source(_ConfigureComponent,_TypedParameterizable):
    def __init__(self,type_,*arg,**kargs):
        super(Source,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        proc._placeSource(name,self)
    def moduleLabel_(self,myname):
        return "@main_input"
    def nameInProcessDesc_(self,myname):
        return "@main_input"


class Looper(_ConfigureComponent,_TypedParameterizable):
    def __init__(self,type_,*arg,**kargs):
        super(Looper,self).__init__(type_,*arg,**kargs)
    def _placeImpl(self,name,proc):
        proc._placeLooper(name,self)
    def moduleLabel_(self,myname):
        return "@main_looper"
    def nameInProcessDesc_(self, myname):
        return "@main_looper"



if __name__ == "__main__":
    import unittest
    from Types import *
    class TestModules(unittest.TestCase):
        def testEDAnalyzer(self):
            empty = EDAnalyzer("Empty")
            withParam = EDAnalyzer("Parameterized",foo=untracked(int32(1)), bar = untracked(string("it")))
            self.assertEqual(withParam.foo.value(), 1)
            self.assertEqual(withParam.bar.value(), "it")
            aCopy = withParam.copy()
            self.assertEqual(aCopy.foo.value(), 1)
            self.assertEqual(aCopy.bar.value(), "it")
            withType = EDAnalyzer("Test",type = int32(1))
            self.assertEqual(withType.type.value(),1)
            block = PSet(i = int32(9))
            m = EDProducer("DumbProducer", block, j = int32(10))
            self.assertEqual(9, m.i.value())
            self.assertEqual(10, m.j.value())
            juicer = ESPrefer("JuiceProducer")
            self.assertEqual(juicer.dumpPython("juicer"), "process.prefer(\"juicer\")")

        
        def testService(self):
            empty = Service("Empty")
            withParam = Service("Parameterized",foo=untracked(int32(1)), bar = untracked(string("it")))
            self.assertEqual(withParam.foo.value(), 1)
            self.assertEqual(withParam.bar.value(), "it")
            self.assertEqual(empty.dumpPython(), "cms.Service(\"Empty\")\n")
            self.assertEqual(withParam.dumpPython(), "cms.Service(\"Parameterized\",\n    foo = cms.untracked.int32(1),\n    bar = cms.untracked.string(\'it\')\n)\n")

    unittest.main()

# Macros {#macros}

## Funciones y métodos obsoletos

Se ha definido la macro *TL_DEPRECATED* para indicar al compilador que muestre un warning para informar que una función o clase esta obsoleta.

Para el caso de funciones se debe incluir la macro antes de la declaración de la función obsoleta añadiendo el método que lo reemplaza:

```
TL_DEPRECATED("NewFunc(int a, float b)")
void OldFunc(int a, float b);
```

En el caso de una clase o un enum se debe añadir entre class y el nombre de la clase:

```
class TL_DEPRECATED(NewClass) OldClass
{ ...
```

En el caso de las funciones template:

```
TL_DEPRECATED("void newFunc(T a, T b)")
void oldFunc(T a, T b);
```

## Mensajes de warning